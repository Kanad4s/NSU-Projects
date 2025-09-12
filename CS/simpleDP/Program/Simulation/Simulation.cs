using DPStrategyContract;
using Simulation.Statistic;

namespace Program.Simulation;

public static class PDSimulation
{
    public static void Simulate(List<Philosopher> philosophers, List<Fork> forks, int steps, IPhilosopherStrategy strategy)
    {
        PrepareSimulation(philosophers, forks);
        for (int i = 0; i < steps; i++)
        {
            SimulationStep(philosophers);
            Statistic.StatusSimulation(i, philosophers, forks);
        }
    }

    public static void SimulationStep(List<Philosopher> philosophers)
    {
        foreach (var p in philosophers)
        {
            // p.Decide();
        }
    }

    private static void PrepareSimulation(List<Philosopher> philosophers, List<Fork> forks)
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