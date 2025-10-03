using System.Runtime.InteropServices.Marshalling;
using DPStrategyContract;

namespace Program.Simulation;

// add table
public class PDSimulation(List<Philosopher> philosophers, List<Fork> forks, IPhilosophersStrategy strategy)
{
    private Statistic _stat = new(philosophers, forks);
    private List<Philosopher> _philosophers = philosophers;
    private List<Fork> _forks = forks;
    private IPhilosophersStrategy _strategy = strategy;
    public void Simulate(int steps)
    {
        var isDeadlock = false;
        int lastStep = 0;
        PrepareSimulation();

        for (int i = 0; i < steps && !isDeadlock; i++)
        {
            lastStep = i;
            // отдельный детектор дедлоков или детект их в мониторе
            isDeadlock = SimulationStep();
            _stat.StepUpdate(i, _philosophers, _forks);
            if (i % 100000 == 0)
            {
                CliStatistic.ShowStatusSimulation(i, _philosophers, _forks);
            }
        }

        CliStatistic.Show(_stat, _philosophers, _forks);

        if (isDeadlock)
        {
            CliStatistic.ShowStatusSimulation(lastStep, _philosophers, _forks);
            CliStatistic.DeadlockShow();
        }
    }

    public bool SimulationStep()
    {
        int philosophersStepped;

        philosophersStepped = _strategy.Step(_philosophers);

        return philosophersStepped == 0;
    }

    private void PrepareSimulation()
    {
        foreach (var p in _philosophers)
        {
            p.StartThinking();
        }
        foreach (var f in _forks)
        {
            f.Release();
        }
    }
}