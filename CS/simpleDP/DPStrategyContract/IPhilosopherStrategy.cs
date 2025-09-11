using DPStrategyContract.Actions;

namespace DPStrategyContract;

public interface IPhilosopherStrategy
{
    void Decide(Philosopher philosopher);
    string StrategyName { get; }

}