using DiningPhilosophers.Contracts;
using DiningPhilosophers.Strategies.Naive;
using FluentAssertions;

namespace DiningPhilosophers.Tests.Strategies;

public class NaiveLeftRightStrategyTests
{
    private readonly NaiveLeftRightStrategy _strategy = new();

    [Fact]
    public void When_not_hungry_returns_None()
    {
        // Arrange
        var perception = new LocalPerception(
            SelfId: 1,
            SelfState: PhilosopherState.Thinking,
            HasLeftFork: false,
            HasRightFork: false,
            LeftForkId: 1,
            LeftForkState: ForkState.Available,
            RightForkId: 2,
            RightForkState: ForkState.Available
        );

        // Act
        var decision = _strategy.Decide(perception);

        // Assert
        decision.Should().Be(ActionDecision.None);
    }

    [Fact]
    public void When_hungry_and_no_forks_takes_left_fork_first()
    {
        // Arrange
        var perception = new LocalPerception(
            SelfId: 1,
            SelfState: PhilosopherState.Hungry,
            HasLeftFork: false,
            HasRightFork: false,
            LeftForkId: 1,
            LeftForkState: ForkState.Available,
            RightForkId: 2,
            RightForkState: ForkState.Available
        );

        // Act
        var decision = _strategy.Decide(perception);

        // Assert
        decision.Should().Be(ActionDecision.TakeLeftFork, 
            "наивная стратегия всегда берет левую вилку первой");
    }

    [Fact]
    public void When_hungry_and_has_left_fork_takes_right_fork()
    {
        // Arrange
        var perception = new LocalPerception(
            SelfId: 1,
            SelfState: PhilosopherState.Hungry,
            HasLeftFork: true,
            HasRightFork: false,
            LeftForkId: 1,
            LeftForkState: ForkState.InUse,
            RightForkId: 2,
            RightForkState: ForkState.Available
        );

        // Act
        var decision = _strategy.Decide(perception);

        // Assert
        decision.Should().Be(ActionDecision.TakeRightFork,
            "после взятия левой вилки должна быть взята правая");
    }

    [Fact]
    public void When_hungry_but_left_fork_unavailable_still_tries_right_fork()
    {
        // Arrange
        var perception = new LocalPerception(
            SelfId: 1,
            SelfState: PhilosopherState.Hungry,
            HasLeftFork: false,
            HasRightFork: false,
            LeftForkId: 1,
            LeftForkState: ForkState.InUse,
            RightForkId: 2,
            RightForkState: ForkState.Available
        );

        // Act
        var decision = _strategy.Decide(perception);

        // Assert
        decision.Should().Be(ActionDecision.TakeRightFork,
            "наивная стратегия берет правую вилку, даже если левая недоступна - это может привести к deadlock");
    }

    [Fact]
    public void When_hungry_and_has_both_forks_returns_None()
    {
        // Arrange
        var perception = new LocalPerception(
            SelfId: 1,
            SelfState: PhilosopherState.Hungry,
            HasLeftFork: true,
            HasRightFork: true,
            LeftForkId: 1,
            LeftForkState: ForkState.InUse,
            RightForkId: 2,
            RightForkState: ForkState.InUse
        );

        // Act
        var decision = _strategy.Decide(perception);

        // Assert
        decision.Should().Be(ActionDecision.None,
            "когда обе вилки уже взяты, дальнейших действий не требуется");
    }

    [Fact]
    public void When_eating_returns_None()
    {
        // Arrange
        var perception = new LocalPerception(
            SelfId: 1,
            SelfState: PhilosopherState.Eating,
            HasLeftFork: true,
            HasRightFork: true,
            LeftForkId: 1,
            LeftForkState: ForkState.InUse,
            RightForkId: 2,
            RightForkState: ForkState.InUse
        );

        // Act
        var decision = _strategy.Decide(perception);

        // Assert
        decision.Should().Be(ActionDecision.None);
    }
}
