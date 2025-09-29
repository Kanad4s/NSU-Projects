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

    public bool IsAvailable()
    {
        return State == ForkState.Available;
    }

    public void Release()
    {
        State = ForkState.Available;
        Owner = null;
    }

    public bool TryUse(Philosopher philosopher)
    {
        if (State == ForkState.InUse)
        {
            return philosopher == Owner;
        }
        else if (State == ForkState.Available)
        {
            Owner = philosopher;
            State = ForkState.InUse;
            return true;
        }
        return false;
    }

}