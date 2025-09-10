using DPStrategyContract.Actions;

namespace DPStrategyContract;

public interface IPhilosopherStrategy
{
    PhilosopherActions Decide(Philosopher philosopher);
    string StrategyName { get; }

}