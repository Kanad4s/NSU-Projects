using System.Collections.Concurrent;
using DiningPhilosophers.Simulator;

namespace DiningPhilosophers.Tests.Simulation.Helpers;

internal sealed class TestRenderer : IRenderer
{
    public readonly ConcurrentBag<(int Step, IReadOnlyList<PhilosopherSnapshot> Philosophers, IReadOnlyList<ForkSnapshot> Forks)> Events = [];

    public void RenderStep(int step, IReadOnlyList<PhilosopherSnapshot> philosophers, IReadOnlyList<ForkSnapshot> forks)
    {
        Events.Add((step, philosophers, forks));
    }
}
