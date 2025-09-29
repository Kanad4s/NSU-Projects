using DPStrategyContract;
namespace DPStrategies;

public class NaiveStrategy : IPhilosophersStrategy
{
    private const int _stepsInactive = 2;
    private Dictionary<Philosopher, int> _deadlockTime;

    public NaiveStrategy(List<Philosopher> philosophers)
    {
        _deadlockTime = [];
        foreach (var p in philosophers)
        {
            _deadlockTime.Add(p, 0);
        }
    }
    public string StrategyName { get; set; } = "Naive Strategy";
    public int Step(List<Philosopher> philosophers)
    {
        int philosophersStepped = 0;
        foreach (var p in philosophers)
        {
            if (!p.Step())
            {
                if (!p.HasLeftFork())
                {
                    if (p.TryTakeLeftFork())
                    {
                        philosophersStepped++;
                        _deadlockTime[p] = 0;
                    }
                }
                else
                {
                    if (p.TryTakeRightFork())
                    {
                        philosophersStepped++;
                        _deadlockTime[p] = 0;
                    }
                    else
                    {
                        _deadlockTime[p]++;
                    }
                }
                if (_deadlockTime[p] > _stepsInactive)
                {
                    Console.WriteLine("Philosopher waits too match");
                    // return false;
                }
                continue;
            }
            philosophersStepped++;
        }

        return philosophersStepped;
    }

}