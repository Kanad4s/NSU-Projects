using DPStrategyContract;
using Simulation.Statistic;

namespace Program.Simulation;

public static class PDSimulation
{
    public static void Simulate(List<Philosopher> philosophers, List<Fork> forks, int steps)
    {
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
            p.Decide();
        }
    }
}