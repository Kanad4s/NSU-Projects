using DiningPhilosophers.Contracts;

namespace DiningPhilosophers.Simulator;

public sealed class Fork
{
    public int Id { get; }
    public ForkState State { get; private set; } = ForkState.Available;
    public int? OwnerId { get; private set; }

    public long TicksAvailable { get; private set; }

    public long TicksBlocked { get; private set; }

    public long TicksEating { get; private set; }

    private bool _inEatingUse;

    public Fork(int id) => Id = id;

    public bool IsFree => State == ForkState.Available;

    public void MarkTick()
    {
        if (State == ForkState.Available) TicksAvailable++;
        else if (_inEatingUse) TicksEating++;
        else TicksBlocked++;
    }

    public bool TryReserve(int philosopherId)
    {
        if (!IsFree) return false;
        State = ForkState.InUse;
        OwnerId = philosopherId;
        _inEatingUse = false;
        return true;
    }

    public void BeginEatingUse(int philosopherId)
    {
        if (OwnerId == philosopherId)
            _inEatingUse = true;
    }

    public void Release()
    {
        State = ForkState.Available;
        OwnerId = null;
        _inEatingUse = false;
    }
}