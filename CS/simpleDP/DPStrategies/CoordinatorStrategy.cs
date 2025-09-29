using DPStrategyContract;
namespace DPStrategies;

public class CoordinatorStrategy : IPhilosophersStrategy
{
    private Dictionary<Philosopher, int> _deadlockTime;
    private Coordinator _coordinator;

    public CoordinatorStrategy(List<Philosopher> philosophers, Coordinator coordinator)
    {
        _deadlockTime = [];
        _coordinator = coordinator;
        foreach (var p in philosophers)
        {
            _deadlockTime.Add(p, 0);
            p.Hungry += _coordinator.TakeHungry;
            _coordinator.TakeFork += (p, f) => p.TryTakeFork(f);
        }
        // _coordinator.TryTakeFork += (p, f) => p.TryTakeFork(f);
    }
    public string StrategyName { get; } = "Coordinator Strategy";
    public int Step(List<Philosopher> philosophers)
    {
        int philosophersStepped = 0;
        foreach (var p in philosophers)
        {
            if (p.StepWithCoordinator())
            {
                philosophersStepped++;
            }
        }
        philosophersStepped += _coordinator.DecideHungry();

        return philosophersStepped;
    }




}