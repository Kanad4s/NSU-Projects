namespace Simulation.Statistic;

public static class Statistic
{
    public static StatusSimulation(List<Philosophers> philosophers, List<Fork> forks)
    {
        StatusPhilosophers(philosophers);
        StatusForks(forks);
    }
    public void StatusPhilosophers(List<Philosophers> philosophers)
    {
        Console.WriteLine("Философы:");
        foreach (var p in philosophers)
        {
            Console.WriteLine($"\t{p._name}: {p._state}, съедено: {p._mealsEaten}");
        }
    }

    public void StatusForks(List<Fork> forks)
    {

    }
}