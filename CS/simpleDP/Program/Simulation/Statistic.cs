using DPStrategyContract;

namespace Simulation.Statistic;

public static class Statistic
{
    public static void StatusSimulation(int step, List<Philosopher> philosophers, List<Fork> forks)
    {
        Console.WriteLine($"===== ШАГ {step} =====");
        StatusPhilosophers(philosophers);
        StatusForks(forks, philosophers);
        Console.WriteLine();
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
                Console.WriteLine($"(используется {f.Owner.Name})");
            }
            Console.WriteLine();
        }
    }
}