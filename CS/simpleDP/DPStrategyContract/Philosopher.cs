using DPStrategyContract.States;

namespace DPStrategyContract;

public class Philosopher(string name)
{
    public string _name { get; } = name;
    private PhilosopherState _state { get; set; }
    private int _mealsEaten { get; set; } = 0;

    // private readonly IPhilosopherStrategy _strategy;

    bool IsThinking { get; }
    bool IsHungry { get; }
    bool IsEating { get; }

    // public Philosopher(string name, PhilosopherState philosopherState)
    // {
    //     _name = name;
    //     _philosopherState = philosopherState;
    //     _mealsEaten = 0;
    // }

    // bool HasLeftFork { get; }
    // bool HasRightFork { get; }
    // bool IsLeftForkAvailable { get; }
    // bool IsRightForkAvailable { get; }

    // nullable 
    // int? RemainingThinkingSteps { get; }
    // int? RemainingEatingSteps { get; }
}