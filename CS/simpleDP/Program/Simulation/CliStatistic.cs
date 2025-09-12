using DPStrategyContract;

namespace Program.Simulation;

public static class CliStatistic
{
    public static void Show(Statistic stat, List<Philosopher> philosophers, List<Fork> forks)
    {
        Console.WriteLine("METRICS:");
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
}