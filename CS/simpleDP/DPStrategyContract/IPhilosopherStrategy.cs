using DPStrategyContract.Actions;

namespace DPStrategyContract;

public interface IPhilosopherStrategy
{
    PhilosopherAction Decide(Philosopher philosopher);
    string StrategyName { get; }

}