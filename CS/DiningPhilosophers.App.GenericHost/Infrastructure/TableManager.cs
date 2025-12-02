using DiningPhilosophers.Contracts;

namespace DiningPhilosophers.App.GenericHost.Infrastructure;

public class TableManager : ITableManager
{
    private readonly object _lock = new();
    private readonly Dictionary<int, int?> _forkOwners = new(); // forkId -> philosopherId?
    private readonly int _philosopherCount;

    public TableManager(int philosopherCount)
    {
        _philosopherCount = philosopherCount;
        for (int i = 1; i <= philosopherCount; i++)
        {
            _forkOwners[i] = null;
        }
    }

    public bool TryAcquireFork(int philosopherId, int forkId)
    {
        lock (_lock)
        {
            if (_forkOwners[forkId] == null)
            {
                _forkOwners[forkId] = philosopherId;
                return true;
            }
            return false;
        }
    }

    public void ReleaseFork(int forkId)
    {
        lock (_lock)
        {
            _forkOwners[forkId] = null;
        }
    }

    public ForkState GetForkState(int forkId)
    {
        lock (_lock)
        {
            return _forkOwners[forkId].HasValue ? ForkState.InUse : ForkState.Available;
        }
    }

    public int GetLeftForkId(int philosopherId)
    {
        return philosopherId;
    }

    public int GetRightForkId(int philosopherId)
    {
        return philosopherId % _philosopherCount + 1;
    }
}
