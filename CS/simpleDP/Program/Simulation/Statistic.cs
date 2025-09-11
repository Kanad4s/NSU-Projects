using DPStrategyContract;

namespace Simulation.Statistic;

public static class Statistic
{
    public static void StatusSimulation(List<Philosopher> philosophers, List<Fork> forks)
    {
        StatusPhilosophers(philosophers);
        StatusForks(forks, philosophers);
    }
    public static void StatusPhilosophers(List<Philosopher> philosophers)
    {
        Console.WriteLine("Философы:");
        foreach (var p in philosophers)
        {
            Console.WriteLine($"\t{p.Name}: {p.State}, съедено: {p.MealsEaten}");
        }
    }

    public static void StatusForks(List<Fork> forks, List<Philosopher> philosophers)
    {
        Console.WriteLine("Вилки:");
        foreach (var f in forks)
        {
            Console.Write($"\tFork-{f.Id}: {f.State} ");
            if (f.Owner != null)
            {
                Console.Write($"(используется {f.Owner.Name})\n");
            }
        }
    }
}