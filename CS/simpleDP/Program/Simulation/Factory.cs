using System.Collections.Generic;
using DPStrategyContract;
using DPStrategyContract.States;
namespace Program.Simulation;

public static class Factory
{
    public static List<Philosopher> CreatePhilosophers(List<string> names, IPhilosopherStrategy strategy, List<Fork> forks)
    {
        var philosophers = new List<Philosopher>();

        for (var i = 0; i < names.Count; i++)
        {
            philosophers.Add(new Philosopher(
                names[i],
                strategy,
                PhilosopherState.Thinking,
                forks[i],
                forks[(i + 1) % forks.Count]
            ));
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
