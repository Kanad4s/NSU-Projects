using DiningPhilosophers.Contracts;
using DiningPhilosophers.Strategies.Naive;
using FluentAssertions;

namespace DiningPhilosophers.Tests.Strategies;

public class ResourceHierarchyStrategyTests
{
    private readonly ResourceHierarchyStrategy _strategy = new();

    [Fact]
    public void When_not_hungry_returns_None()
    {
        // Arrange
        var perception = new LocalPerception(
            SelfId: 2,
            SelfState: PhilosopherState.Thinking,
            HasLeftFork: false,
            HasRightFork: false,
            LeftForkId: 2,
            LeftForkState: ForkState.Available,
            RightForkId: 3,
            RightForkState: ForkState.Available
        );

        // Act
        var decision = _strategy.Decide(perception);

        // Assert
        decision.Should().Be(ActionDecision.None);
    }

    [Fact]
    public void When_left_fork_has_lower_id_takes_left_fork_first()
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
            "если левая вилка имеет меньший ID, она должна быть взята первой");
    }

    [Fact]
    public void When_right_fork_has_lower_id_takes_right_fork_first()
    {
        // Arrange
        var perception = new LocalPerception(
            SelfId: 3,
            SelfState: PhilosopherState.Hungry,
            HasLeftFork: false,
            HasRightFork: false,
            LeftForkId: 3,
            LeftForkState: ForkState.Available,
            RightForkId: 1,
            RightForkState: ForkState.Available
        );
        
        // Act
        var decision = _strategy.Decide(perception);

        // Assert
        decision.Should().Be(ActionDecision.TakeRightFork,
            "если правая вилка имеет меньший ID, она должна быть взята первой");
    }

    [Fact]
    public void When_has_lower_id_fork_takes_higher_id_fork_next()
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
            "после взятия вилки с меньшим ID должна быть взята вилка с большим ID");
    }

    [Fact]
    public void When_lower_id_fork_unavailable_can_take_higher_id_fork()
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
        decision.Should().Be(ActionDecision.None,
            "стратегия не может взять вилку с большим ID, если вилка с меньшим ID недоступна");
    }

    [Fact]
    public void When_right_fork_has_lower_id_and_has_it_takes_left_fork_next()
    {
        // Arrange
        var perception = new LocalPerception(
            SelfId: 3,
            SelfState: PhilosopherState.Hungry,
            HasLeftFork: false,
            HasRightFork: true,
            LeftForkId: 3,
            LeftForkState: ForkState.Available,
            RightForkId: 1,
            RightForkState: ForkState.InUse
        );

        // Act
        var decision = _strategy.Decide(perception);

        // Assert
        decision.Should().Be(ActionDecision.TakeLeftFork,
            "после взятия вилки с меньшим ID (правой) должна быть взята вилка с большим ID (левая)");
    }

    [Fact]
    public void When_has_both_forks_returns_None()
    {
        // Arrange
        var perception = new LocalPerception(
            SelfId: 2,
            SelfState: PhilosopherState.Hungry,
            HasLeftFork: true,
            HasRightFork: true,
            LeftForkId: 2,
            LeftForkState: ForkState.InUse,
            RightForkId: 3,
            RightForkState: ForkState.InUse
        );
        
        // Act
        var decision = _strategy.Decide(perception);

        // Assert
        decision.Should().Be(ActionDecision.None,
            "когда обе вилки уже взяты, дальнейших действий не требуется");
    }

    [Fact]
    public void Strategy_breaks_symmetry_for_circular_deadlock_prevention()
    {
        // Arrange - проверяем, что разные философы берут вилки в разном порядке
        // Философ 1: левая=1, правая=2 → берет левую 
        var p1 = new LocalPerception(1, PhilosopherState.Hungry, false, false, 1, ForkState.Available, 2, ForkState.Available);
        
        // Философ 2: левая=2, правая=3 → берет левую 
        var p2 = new LocalPerception(2, PhilosopherState.Hungry, false, false, 2, ForkState.Available, 3, ForkState.Available);
        
        // Философ 3: левая=3, правая=1 → берет правую 
        var p3 = new LocalPerception(3, PhilosopherState.Hungry, false, false, 3, ForkState.Available, 1, ForkState.Available);

        // Act
        var d1 = _strategy.Decide(p1);
        var d2 = _strategy.Decide(p2);
        var d3 = _strategy.Decide(p3);

        // Assert
        d1.Should().Be(ActionDecision.TakeLeftFork);
        d2.Should().Be(ActionDecision.TakeLeftFork);
        d3.Should().Be(ActionDecision.TakeRightFork, 
            "последний философ должен брать вилку в другом порядке");
    }
}
