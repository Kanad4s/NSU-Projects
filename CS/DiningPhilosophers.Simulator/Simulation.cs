using DiningPhilosophers.Contracts;

namespace DiningPhilosophers.Simulator;

public sealed class Simulation
{
    private readonly DefaultRng _rng;
    private readonly IRenderer? _renderer;
    private readonly List<Fork> _forks = [];
    private readonly List<Philosopher> _philosophers = [];

    private long _stepsRun;
    
    // детектор дедлока
    private int  _noProgressSteps;
    private const int DeadlockGrace = 10; 
    private bool _deadlockDetected;
    private int  _deadlockAtStep;
    private int  _prevTotalEaten;
    
    public Simulation(IReadOnlyList<string> names, DefaultRng? rng = null, IRenderer? renderer = null)
    {
        _rng = rng ?? new DefaultRng();
        _renderer = renderer;

        for (var i = 0; i < names.Count; i++)
        {
            _forks.Add(new Fork(i + 1));
        }

        for (var i = 0; i < names.Count; i++)
        {
            var left = _forks[i];
            var right = _forks[(i + 1) % names.Count];
            _philosophers.Add(new Philosopher(i + 1, names[i], left, right, _rng));
        }
    }

    public bool Run(IPhilosopherStrategy strategy, int steps, int renderEvery, ICoordinator? coordinator = null)
    {
        for (int step = 1; step <= steps; step++)
        {
            TickForks();

            var prevStates = SnapshotStates();

            TickPhilosophersBeforeDecision();

            NotifyCoordinatorTransitions(coordinator, prevStates);

            ApplyStrategy(strategy);

            RenderIfNeeded(renderEvery, step);

            if (CheckDeadlock(step))
            {
                return true;
            }

            _stepsRun = step; 
        }

        return false;
    }

    private void TickForks()
    {
        foreach (var f in _forks) f.MarkTick();
    }

    private PhilosopherState[] SnapshotStates()
    {
        return _philosophers.Select(p => p.State).ToArray();
    }

    private void TickPhilosophersBeforeDecision()
    {
        foreach (var p in _philosophers) p.TickBeforeDecision();
    }

    private void NotifyCoordinatorTransitions(ICoordinator? coordinator, PhilosopherState[] prevStates)
    {
        if (coordinator == null) return;
        for (int i = 0; i < _philosophers.Count; i++)
        {
            var was = prevStates[i];
            var now = _philosophers[i].State;
            var id  = _philosophers[i].Id;

            if (was != PhilosopherState.Hungry && now == PhilosopherState.Hungry)
                coordinator.RequestToEat(id);

            if (was == PhilosopherState.Eating && now == PhilosopherState.Thinking)
                coordinator.SeatFreed(id);
        }
    }

    private void ApplyStrategy(IPhilosopherStrategy strategy)
    {
        foreach (var p in _philosophers) p.DecideAndAct(strategy);
    }

    private void RenderIfNeeded(int renderEvery, int step)
    {
        if (_renderer == null || step % renderEvery != 0) return;
        var ph = _philosophers.Select(p => p.Snapshot()).ToList();
        var fk = _forks.Select(f => new ForkSnapshot(f.Id, f.State, f.OwnerId)).ToList();
        _renderer.RenderStep(step, ph, fk);
    }

    private bool CheckDeadlock(int step)
    {
        var totalEatenNow = _philosophers.Sum(p => p.EatenCount);
        if (totalEatenNow == _prevTotalEaten)
        {
            _noProgressSteps++;
        }
        else
        {
            _noProgressSteps = 0; _prevTotalEaten = totalEatenNow;
        }

        if (_noProgressSteps >= DeadlockGrace
            && _philosophers.All(p => p.State == PhilosopherState.Hungry)
            && _forks.All(f => f.State == ForkState.InUse))
        {
            _deadlockDetected = true;
            _deadlockAtStep = step;
            
            if (_renderer != null)
            {
                var ph = _philosophers.Select(p => p.Snapshot()).ToList();
                var fk = _forks.Select(f => new ForkSnapshot(f.Id, f.State, f.OwnerId)).ToList();
                _renderer.RenderStep(step, ph, fk);
            }

            _stepsRun = step;
            return true; 
        }
        return false;
    }
    
    public void PrintFinalReport()
    {
        var totalSteps = Math.Max(1, (int)_stepsRun); 
        var n = _philosophers.Count;

        Console.WriteLine();
        Console.WriteLine("===== FINAL REPORT =====");

        var totalEaten = _philosophers.Sum(p => p.EatenCount);
        Console.WriteLine($"Score (total eaten): {totalEaten}");

        Console.WriteLine();
        Console.WriteLine("Throughput (per 1000 steps):");
        double sumTp = 0;
        foreach (var p in _philosophers)
        {
            var tp = p.EatenCount * 1000.0 / totalSteps;
            sumTp += tp;
            Console.WriteLine($"  {p.Name}: {tp:F2}");
        }
        Console.WriteLine($"  Average: {(sumTp / n):F2}");

        Console.WriteLine();
        Console.WriteLine("Waiting time (Hungry):");
        foreach (var p in _philosophers)
        {
            var avgWait = p.HungryEpisodes > 0
                ? (double)p.TotalHungrySteps / p.HungryEpisodes
                : 0.0;
            Console.WriteLine($"  {p.Name}: avg={avgWait:F2} steps, max={p.MaxWaitingInHungry} steps, episodes={p.HungryEpisodes}");
        }
        var worst = _philosophers.OrderByDescending(p => p.MaxWaitingInHungry).First();
        Console.WriteLine($"Max waiting overall: {worst.Name} = {worst.MaxWaitingInHungry} steps");

        Console.WriteLine();
        Console.WriteLine("Fork utilization:");
        foreach (var f in _forks)
        {
            double freePct   = 100.0 * f.TicksAvailable / totalSteps;
            double blockedPct= 100.0 * f.TicksBlocked   / totalSteps;
            double eatingPct = 100.0 * f.TicksEating    / totalSteps;
            Console.WriteLine($"  Fork-{f.Id}: free={freePct:F1}% blocked={blockedPct:F1}% eating={eatingPct:F1}%");
        }
    }

}