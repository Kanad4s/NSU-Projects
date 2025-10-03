using DPStrategyContract;
using DPStrategyContract.States;

namespace Program.Simulation;

public static class CliStatistic
{
    public static void Show(Statistic stat, List<Philosopher> philosophers, List<Fork> forks)
    {
        Console.WriteLine("METRICS:");
        Console.WriteLine($"Steps: {stat.Steps}");
        Console.WriteLine("Пропускная способность:");
        foreach (var p in philosophers)
        {
            Console.WriteLine($"\t{p.Name}: {stat.MealsEaten[p.Name]}");
        }
        Console.WriteLine("Время ожидания:");
        foreach (var p in philosophers)
        {
            Console.WriteLine($"\t{p.Name}: {stat.PhilosopherWaiting[p.Name]}");
        }
        Console.WriteLine("Коэффициент утлилизации:");
        foreach (var f in forks)
        {
            Console.WriteLine($"\tFork-{f.Id}:");
            Console.WriteLine($"\t   Available: {stat.ForkAvailable[f.Id]} steps out of {stat.Steps}");
            Console.WriteLine($"\t   Blocked: {stat.ForkBlocked[f.Id]} steps out of {stat.Steps}");
            Console.WriteLine($"\t   Eat: {stat.Steps - stat.ForkBlocked[f.Id] - stat.ForkAvailable[f.Id]} steps out of {stat.Steps}");
        }
    }

    public static void DeadlockShow()
    {
        Console.WriteLine("DEADLOCK");
        Console.WriteLine("DEADLOCK");
        Console.WriteLine("DEADLOCK");
    }

    public static void ShowStatusSimulation(int step, List<Philosopher> philosophers, List<Fork> forks)
    {
        // Console.Clear();
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
            if (f.IsInUse())
            {
                Console.WriteLine($"(используется {f.Owner?.Name})");
            }
            Console.WriteLine();
        }
    }

}