namespace Program.Simulation;

public static class Simulation
{
    public static void Simulate(List<Philosopher> philosophers, int steps)
    {
        for (int i = 0; i < steps; i++)
        {
            SimulationStep(philosophers);
            Statistic.StatusSimulation(philosophers);
        }
    }

    public void SimulationStep(List<Philosopher> philosophers)
    {
        foreach (var p in philosophers)
        {
            p.Decide();
        }
    }
}