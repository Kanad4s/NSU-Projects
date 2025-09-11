using DPStrategyContract.States;

namespace DPStrategyContract;

public class Fork(int id)
{
    private int _id { get; } = id;

    private ForkState _state { get; set; }

}