using DiningPhilosophers.Contracts;

namespace DiningPhilosophers.Simulator;

public sealed record PhilosopherSnapshot(
    int Id,
    string Name,
    PhilosopherState State,
    bool HasLeftFork,
    bool HasRightFork,
    int? EatingStepsLeft,
    int? ThinkingStepsLeft,
    string? ActionHint,
    int EatenCount,
    int WaitingInHungry);

public sealed record ForkSnapshot(
    int Id,
    ForkState State,
    int? OwnerPhilosopherId);