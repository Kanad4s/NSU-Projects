using DPStrategyContract;
using DPStrategyContract.States;

namespace Simulation.Statistic;

public static class Statistic
{
    public static void StatusSimulation(int step, List<Philosopher> philosophers, List<Fork> forks)
    {
        Console.WriteLine($"===== ШАГ {step} =====");
        StatusPhilosophers(philosophers);
        StatusForks(forks);
        Console.WriteLine();
    }
    public static void StatusPhilosophers(List<Philosopher> philosophers)
    {
        Console.WriteLine("Философы:");
        foreach (var p in philosophers)
        {
            Console.Write($"\t{p.Name}: {p.State}");
            if (p.State == PhilosopherState.Thinking || p.State == PhilosopherState.Eating)
            {
                Console.Write($" ({p.StateDuration} steps left)");
            }
            Console.WriteLine($", съедено: {p.MealsEaten}");
        }
    }

    public static void StatusForks(List<Fork> forks)
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