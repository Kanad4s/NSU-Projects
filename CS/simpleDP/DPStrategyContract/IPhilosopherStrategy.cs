namespace DPStrategyContract;

public interface IPhilosopherStrategy
{
    PhilosopherActions Decide(Philosopher philosopher);
    string Name { get; }

}