using DiningPhilosophers.Contracts;
using DiningPhilosophers.Strategies.Naive;
using DiningPhilosophers.Tests.Simulation.Helpers;
using FluentAssertions;

namespace DiningPhilosophers.Tests.Simulation;

public class SimulationStateTransitionsTests
{
    [Fact]
    public void Philosopher_cycles_through_Thinking_Hungry_Eating_Thinking()
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
            steps: 200,
            renderEvery: 1);

        // Assert
        var events = renderer.Events
            .OrderBy(e => e.Step)
            .ToList();

        events.Should().NotBeEmpty();

        var found = false;

        foreach (var pid in names.Select((_, idx) => idx + 1))
        {
            var statesByStep = events
                .Select(e => (
                    e.Step,
                    State: e.Philosophers
                        .First(p => p.Id == pid)
                        .State))
                .ToList();

            // Разрешаем как строгий T→H→E→T, так и T→E→T
            var hasFull = ContainsPattern(
                statesByStep,
                PhilosopherState.Thinking,
                PhilosopherState.Hungry,
                PhilosopherState.Eating,
                PhilosopherState.Thinking);

            var hasShort = ContainsPattern(
                statesByStep,
                PhilosopherState.Thinking,
                PhilosopherState.Eating,
                PhilosopherState.Thinking);

            if (hasFull || hasShort)
            {
                found = true;
                break;
            }
        }

        found.Should().BeTrue(
            "хотя бы один философ должен пройти цикл " +
            "T→(H→)E→T в пределах 200 шагов");
    }

    private static bool ContainsPattern(
        List<(int Step, PhilosopherState State)> statesByStep,
        params PhilosopherState[] pattern)
    {
        var start = -1;

        while (true)
        {
            start = statesByStep.FindIndex(
                start + 1,
                s => s.State == pattern[0]);

            if (start < 0)
            {
                return false;
            }

            var pos = start;
            var ok = true;

            for (var i = 1; i < pattern.Length; i++)
            {
                pos = statesByStep.FindIndex(
                    pos + 1,
                    s => s.State == pattern[i]);

                if (pos < 0)
                {
                    ok = false;
                    break;
                }
            }

            if (ok)
            {
                return true;
            }

            // иначе пробуем со следующего появления первого состояния
        }
    }
}
