using DiningPhilosophers.Contracts;

namespace DiningPhilosophers.Simulator.Concurrent;

public sealed class ConcurrentFork
{
    private readonly object _sync = new();

    public int Id { get; }
    public ForkState State { get; private set; } = ForkState.Available;
    public int? OwnerId { get; private set; }

    // true, если вилка прямо сейчас участвует в еде (для метрик)
    private bool _inEatingUse;

    public long FreeMs { get; private set; }
    public long BlockedMs { get; private set; }
    public long EatingMs { get; private set; }

    private long _lastChangeMs;

    public ConcurrentFork(int id, long nowMs)
    {
        Id = id;
        _lastChangeMs = nowMs;
        State = ForkState.Available;
        OwnerId = null;
        _inEatingUse = false;
    }

    public bool TryAcquire(int philosopherId, long nowMs)
    {
        lock (_sync)
        {
            if (State != ForkState.Available)
                return false;

            AccumulateSinceLast(nowMs);
            State = ForkState.InUse;
            OwnerId = philosopherId;
            _inEatingUse = false;
            return true;
        }
    }

    public void BeginEatingUse(int philosopherId, long nowMs)
    {
        lock (_sync)
        {
            if (State != ForkState.InUse || OwnerId != philosopherId)
                return;

            if (_inEatingUse)
                return;

            AccumulateSinceLast(nowMs);
            _inEatingUse = true;
        }
    }

    public void Release(long nowMs)
    {
        lock (_sync)
        {
            if (State == ForkState.Available)
                return;

            AccumulateSinceLast(nowMs);
            State = ForkState.Available;
            OwnerId = null;
            _inEatingUse = false;
        }
    }

    public void AccumulateToNow(long nowMs)
    {
        lock (_sync)
        {
            AccumulateSinceLast(nowMs);
        }
    }

    private void AccumulateSinceLast(long nowMs)
    {
        var dt = nowMs - _lastChangeMs;
        if (dt < 0) dt = 0;

        if (State == ForkState.Available) FreeMs += dt;
        else if (_inEatingUse) EatingMs += dt;
        else BlockedMs += dt;

        _lastChangeMs = nowMs;
    }

    public ForkState ReadState()
    {
        lock (_sync) return State;
    }

    public int? ReadOwnerId()
    {
        lock (_sync) return OwnerId;
    }

    public bool ReadInEatingUse()
    {
        lock (_sync) return _inEatingUse;
    }

    // получаем Snapshot
    public ConcurrentForkSnapshot Snapshot(long nowMs)
    {
        lock (_sync)
        {
            AccumulateSinceLast(nowMs);
            return new ConcurrentForkSnapshot(
                Id, State, OwnerId, _inEatingUse,
                FreeMs, BlockedMs, EatingMs
            );
        }
    }
}

// Snapshot
public readonly record struct ConcurrentForkSnapshot(
    int Id,
    ForkState State,
    int? OwnerId,
    bool InEatingUse,
    long FreeMs,
    long BlockedMs,
    long EatingMs
);
