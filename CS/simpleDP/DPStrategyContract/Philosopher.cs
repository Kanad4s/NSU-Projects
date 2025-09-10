using DPStrategyContract.States;

namespace DPStrategyContract;

public class Philosopher(string name, IPhilosopherStrategy strategy)
{
    public string _name { get; set; } = name;
    public PhilosopherState _state { get; set; }
    public IPhilosopherStrategy _strategy { get; set; } = strategy;
    public int _mealsEaten { get; set; } = 0;


    public void Decide()
    {
        _strategy.Decide(this);
    }


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