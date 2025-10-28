namespace DiningPhilosophers.Contracts;

public readonly record struct LocalPerception(
    int SelfId,
    PhilosopherState SelfState,
    bool HasLeftFork,
    bool HasRightFork,
    int LeftForkId,
    ForkState LeftForkState,
    int RightForkId,
    ForkState RightForkState);