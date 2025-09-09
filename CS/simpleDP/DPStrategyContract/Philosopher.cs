namespace DPStrategyContract;

public class Philosopher
{
    public string _name { get; }
    private PhilosopherState _state { get; set; }
    private int _mealsEaten { get; set; }

    // private readonly IPhilosopherStrategy _strategy;

    bool IsThinking { get; }
    bool IsHungry { get; }
    bool IsEating { get; }

     public Philosopher(string name)
    {
        _name = name;
        _mealsEaten = 0;
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