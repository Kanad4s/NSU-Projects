using System.Runtime.InteropServices.Marshalling;
using DPStrategyContract;

namespace Program.Simulation;

public  class PDSimulation(List<Philosopher> philosophers, List<Fork> forks)
{
    private  Statistic _stat = new(philosophers, forks);
    public  void Simulate(List<Philosopher> philosophers, List<Fork> forks, int steps, IPhilosopherStrategy strategy)
    {
        PrepareSimulation(philosophers, forks);
        for (int i = 0; i < steps; i++)
        {
            SimulationStep(philosophers, strategy);
            _stat.StepUpdate(i, philosophers, forks);
            _stat.StatusSimulation(i, philosophers, forks);
        }
        CliStatistic.Show(_stat, philosophers, forks);
    }

    public  void SimulationStep(List<Philosopher> philosophers, IPhilosopherStrategy strategy)
    {
        foreach (var p in philosophers)
        {
            p.Step();
            strategy.Decide(p);
        }
    }

    private  void PrepareSimulation(List<Philosopher> philosophers, List<Fork> forks)
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