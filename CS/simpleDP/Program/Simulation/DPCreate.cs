using System.Collections.Generic;
using DPStrategyContract;
namespace Program.Simulation;

public static class PhilosopherFactory
{
    public static List<Philosopher> CreatePhilosophers(IEnumerable<string> names)
    {
        var philosophers = new List<Philosopher>();

        foreach (var name in names)
        {
            philosophers.Add(new Philosopher(name));
        }

        return philosophers;
    }
    
}
