using DiningPhilosophers.Simulator;
using DiningPhilosophers.Strategies.Naive;
using DiningPhilosophers.Tests.Simulation.Helpers;
using FluentAssertions;

namespace DiningPhilosophers.Tests.Deadlock;

public class DeadlockDetectionTests
{
    
    [Fact]
    public void Deadlock_is_detected_when_simulation_is_stuck()
    {
        
        var names = new[] { "A", "B", "С" };
        DefaultRng rng = new FixedRng(1);
        var renderer = new TestRenderer();
        var sim = new Simulator.Simulation(names, rng, renderer);
        var strategy = new NaiveLeftRightStrategy();

        var caughtDeadlock = sim.Run(strategy, steps: 1000, renderEvery: 50);
        
        caughtDeadlock.Should().BeTrue("тест на дедлок для наивной стратегии должен сработать");
    }
}
