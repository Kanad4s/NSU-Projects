using System.Runtime.InteropServices.Marshalling;
using DPStrategyContract;

namespace Program.Simulation;

public class PDSimulation(List<Philosopher> philosophers, List<Fork> forks)
{
    private  Statistic _stat = new(philosophers, forks);
    public void Simulate(List<Philosopher> philosophers, List<Fork> forks, int steps, IPhilosopherStrategy strategy)
    {
        var isDeadlock = false;
        int lastStep = 0;
        PrepareSimulation(philosophers, forks);
        for (int i = 0; i < steps && !isDeadlock; i++)
        {
            lastStep = i;
            isDeadlock = SimulationStep(philosophers, strategy);
            _stat.StepUpdate(i, philosophers, forks);
            if (i % 100000 == 0)
            {
                _stat.ShowStatusSimulation(i, philosophers, forks);
            }
        }
        CliStatistic.Show(_stat, philosophers, forks);
        if (isDeadlock)
        {
            _stat.ShowStatusSimulation(lastStep, philosophers, forks);
            CliStatistic.DeadlockShow();
        }
    }

    public bool SimulationStep(List<Philosopher> philosophers, IPhilosopherStrategy strategy)
    {
        int isDeadlock = 0;
        foreach (var p in philosophers)
        {
            if (!strategy.Decide(p))
            {
                isDeadlock++;
            }
        }
        if (isDeadlock == philosophers.Count)
        {
            return true;
        }

        return false;
    }

    private void PrepareSimulation(List<Philosopher> philosophers, List<Fork> forks)
    {
        foreach (var p in philosophers)
        {
            p.StartThinking();
        }
        foreach (var f in forks)
        {
            f.Release();
        }
    }
}