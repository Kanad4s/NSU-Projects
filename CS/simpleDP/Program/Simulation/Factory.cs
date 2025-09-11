using System.Collections.Generic;
using DPStrategyContract;
namespace Program.Simulation;

public static class Factory
{
    public static List<Philosopher> CreatePhilosophers(IEnumerable<string> names, IPhilosopherStrategy strategy)
    {
        var philosophers = new List<Philosopher>();

        foreach (var name in names)
        {
            philosophers.Add(new Philosopher(name, strategy));
        }

        return philosophers;
    }

    public static List<Fork> CreateForks(int count)
    {
        var forks = new List<Fork>();

        for (var i = 0; i < count; i++)
        {
            forks.Add(new Fork(i));
        }

        return forks;
    }
    
}
