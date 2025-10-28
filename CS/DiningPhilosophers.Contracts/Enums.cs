namespace DiningPhilosophers.Contracts;


public enum PhilosopherState { Thinking, Hungry, Eating }
public enum ForkState { Available, InUse }

public enum ForkSide { Left, Right }

public enum ActionDecision
{
    None,
    
    TakeLeftFork,
    TakeRightFork,
    ReleaseLeftFork,
    ReleaseRightFork
}