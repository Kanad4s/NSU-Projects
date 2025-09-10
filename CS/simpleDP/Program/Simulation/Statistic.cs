using DPStrategyContract;

namespace Simulation.Statistic;

public static class Statistic
{
    public static void StatusSimulation(List<Philosopher> philosophers, List<Fork> forks)
    {
        StatusPhilosophers(philosophers);
        StatusForks(forks);
    }
    public static void StatusPhilosophers(List<Philosopher> philosophers)
    {
        Console.WriteLine("Философы:");
        foreach (var p in philosophers)
        {
            Console.WriteLine($"\t{p._name}: {p._state}, съедено: {p._mealsEaten}");
        }
    }

    public static void StatusForks(List<Fork> forks)
    {

    }
}