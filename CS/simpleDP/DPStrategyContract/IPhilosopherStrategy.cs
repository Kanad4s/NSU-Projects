using DPStrategyContract.Actions;

namespace DPStrategyContract;

public interface IPhilosopherStrategy
{
    bool Decide(Philosopher philosopher);
    string StrategyName { get; }

}