using System.Runtime.InteropServices.Marshalling;
using DPStrategyContract;
using Program.Infrastructure.Config;

namespace Program.Simulation;

// add table
public class PDSimulation
{
    private Statistic _stat;
    private Table _table;
    private List<Philosopher> _philosophers;
    private List<Fork> _forks;
    private IPhilosophersStrategy _strategy;

    public PDSimulation(List<Philosopher> philosophers, List<Fork> forks, IPhilosophersStrategy strategy)
    {
        _stat = new(philosophers, forks);
        _philosophers = philosophers;
        _forks = forks;
        _strategy = strategy;
    }

    public PDSimulation(AppConfig config)
    {
        var names = new FilePhilosopherNameProvider(config.PhilosophersFile);

        _forks = Factory.CreateForks(names.GetNames().Count());

        _table = new(_forks);

        _philosophers = Factory.CreatePhilosophers([.. names.GetNames()], _forks, config);

        _table.SitPhilosophers(_philosophers);

    }
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