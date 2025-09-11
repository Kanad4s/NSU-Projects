using DPStrategyContract.States;

namespace DPStrategyContract;

public class Philosopher
{
    public string Name { get; }
    public PhilosopherState State { get; set; }
    public IPhilosopherStrategy Strategy { get; set; }

    public int MealsEaten = 0;
    public Fork LeftFork { get; set; }
    public Fork RightFork { get; set; }

    public Philosopher(string name, IPhilosopherStrategy strategy, PhilosopherState state, Fork leftFork, Fork rightFork)
    {
        Name = name;
        Strategy = strategy;
        State = state;
        LeftFork = leftFork;
        RightFork = rightFork;
    }


    public void Decide()
    {
        Strategy.Decide(this);
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