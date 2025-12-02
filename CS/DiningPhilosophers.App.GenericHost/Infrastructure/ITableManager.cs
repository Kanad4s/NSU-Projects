using DiningPhilosophers.Contracts;

namespace DiningPhilosophers.App.GenericHost.Infrastructure;

public interface ITableManager
{
    bool TryAcquireFork(int philosopherId, int forkId);
    void ReleaseFork(int forkId);
    ForkState GetForkState(int forkId);
    int GetLeftForkId(int philosopherId);
    int GetRightForkId(int philosopherId);
}

