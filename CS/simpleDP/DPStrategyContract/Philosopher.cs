namespace DPStrategyContract;

public class Philosopher
{
    private int Id { get; }
    private string Name { get; }
    private PhilosopherState philosopherState { get; }
    private int MealsEaten { get; set; }

    private readonly IPhilosopherStrategy _strategy;

    bool IsThinking { get; }
    bool IsHungry { get; }
    bool IsEating { get; }

    // bool HasLeftFork { get; }
    // bool HasRightFork { get; }
    // bool IsLeftForkAvailable { get; }
    // bool IsRightForkAvailable { get; }

    // nullable 
    // int? RemainingThinkingSteps { get; }
    // int? RemainingEatingSteps { get; }
}