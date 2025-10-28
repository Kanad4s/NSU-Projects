namespace DiningPhilosophers.Contracts;

// Любая стратегия принимает решение, только на основе LocalPerception
public interface IPhilosopherStrategy
{
    ActionDecision Decide(in LocalPerception view);
}