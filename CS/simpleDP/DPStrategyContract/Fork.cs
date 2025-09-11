using DPStrategyContract.States;
using Microsoft.Win32.SafeHandles;

namespace DPStrategyContract;

public class Fork(int id)
{
    public int Id { get; } = id;

    public ForkState State { get; private set; }

    public Philosopher? Owner { get; private set; }

    public bool IsInUse()
    {
        return State == ForkState.InUse;
    }



}