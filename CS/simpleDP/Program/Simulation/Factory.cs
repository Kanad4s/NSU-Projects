using System.Collections.Generic;
using DPStrategyContract;
using DPStrategyContract.States;
using Program.Infrastructure.Config;
namespace Program.Simulation;

public static class Factory
{
    public static List<Philosopher> CreatePhilosophers(List<string> names, List<Fork> forks, AppConfig config)
    {
        var philosophers = new List<Philosopher>();

        for (var i = 0; i < names.Count; i++)
        {
            philosophers.Add(new Philosopher(
                names[i],
                PhilosopherState.Thinking,
                forks[i],
                forks[(i + 1) % forks.Count],
                config.Simulation.ThinkingMin,
                config.Simulation.ThinkingMax,
                config.Simulation.EatingMin,
                config.Simulation.EatingMax
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
