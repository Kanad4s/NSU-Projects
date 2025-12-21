using DiningPhilosophers.Contracts;
using DiningPhilosophers.Strategies.Naive;
using DiningPhilosophers.Tests.Simulation.Helpers;
using FluentAssertions;
using Xunit.Sdk;

namespace DiningPhilosophers.Tests.Simulation;

public class SimulationForkOwnershipConsistencyTests
{
    // Проверяет, что когда философ ест, обе соседние вилки принадлежат ему
    [Fact]
    public void When_philosopher_is_eating_both_adjacent_forks_are_owned_by_him_and_released_after()
    {
        // Arrange
        var names = new[] { "A", "B", "C" };
        var renderer = new TestRenderer();
        var sim = new Simulator.Simulation(
            names,
            rng: null,
            renderer);
        var strategy = new NaiveLeftRightStrategy();

        // Act
        sim.Run(
            strategy,
            steps: 300,
            renderEvery: 1);

        // Assert
        var events = renderer.Events
            .OrderBy(e => e.Step)
            .ToList();

        for (var i = 1; i <= names.Length; i++)
        {
            var pid = i;

            var states = events
                .Select(e => (
                    e.Step,
                    P: e.Philosophers.First(p => p.Id == pid)))
                .ToList();

            var eatIdx = states.FindIndex(
                s => s.P.State == PhilosopherState.Eating);

            if (eatIdx < 0)
            {
                continue;
            }

            var eatStep = states[eatIdx].Step;
            var forksAtEat = events
                .First(e => e.Step == eatStep)
                .Forks;

            var n = names.Length;
            var leftId = pid;
            var rightId = pid % n + 1;

            var left = forksAtEat.First(f => f.Id == leftId);
            var right = forksAtEat.First(f => f.Id == rightId);

            left.State.Should().Be(ForkState.InUse);
            right.State.Should().Be(ForkState.InUse);
            left.OwnerPhilosopherId.Should().Be(pid);
            right.OwnerPhilosopherId.Should().Be(pid);

            // Найти момент возврата к Thinking
            var backIdx = states.FindIndex(
                eatIdx + 1,
                s => s.P.State == PhilosopherState.Thinking);

            backIdx.Should().BeGreaterThan(0);

            var backStep = states[backIdx].Step;
            var forksAfter = events
                .First(e => e.Step == backStep)
                .Forks;

            var leftAfter = forksAfter.First(f => f.Id == leftId);
            var rightAfter = forksAfter.First(f => f.Id == rightId);

            leftAfter.OwnerPhilosopherId.Should().NotBe(pid);
            rightAfter.OwnerPhilosopherId.Should().NotBe(pid);

            return;
        }

        throw new XunitException(
            "Не найден момент начала еды у любого философа");
    }
}
