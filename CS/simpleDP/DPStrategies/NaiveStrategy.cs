using DPStrategyContract;
using DPStrategyContract.Actions;
using DPStrategyContract.States;
namespace DPStrategies;

public class NaiveStrategy() : IPhilosopherStrategy
{
    public string StrategyName { get; set; } = "Naive Strategy";
    public PhilosopherAction Decide(Philosopher philosopher)
    {
        philosopher.State = PhilosopherState.Thinking;
        return PhilosopherAction.TakeLeftFork;
    }

}