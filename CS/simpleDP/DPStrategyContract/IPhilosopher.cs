
namespace DPStrategyContract;

public interface IPhilosopher
{
    int Id { get; }
    string Name { get; }

    bool IsThinking { get; }
    bool IsHungry { get; }
    bool IsEating { get; }

    bool HasLeftFork { get; }
    bool HasRightFork { get; }
    bool IsLeftForkAvailable { get; }
    bool IsRightForkAvailable { get; }

    // nullable 
    int? RemainingThinkingSteps { get; }
    int? RemainingEatingSteps { get; }
}