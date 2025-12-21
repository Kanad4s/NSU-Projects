using DiningPhilosophers.Simulator;

namespace DiningPhilosophers.Tests.Deadlock;

public sealed class FixedRng : DefaultRng
{
    private readonly int _value;

    public FixedRng(int value)
    {
        _value = value;
    }

    public override int NextInclusive(int min, int max) => _value;
}