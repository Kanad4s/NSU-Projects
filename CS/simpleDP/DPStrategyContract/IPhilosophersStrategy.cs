namespace DPStrategyContract;

public interface IPhilosophersStrategy
{
    int Step(List<Philosopher> philosophers);
    string StrategyName { get; }

}

public delegate void OnHungry(Philosopher philosopher);
public delegate bool TryTakeFork(Philosopher philosopher, Fork fork);