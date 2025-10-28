namespace DiningPhilosophers.Simulator;

public class DefaultRng(int? seed = null)
{
    private readonly Random _r = seed.HasValue ? new Random(seed.Value) : new Random();

    public virtual int NextInclusive(int min, int max) => _r.Next(min, max + 1);
}