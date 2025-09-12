using DPStrategyContract;
using DPStrategyContract.Actions;
using DPStrategyContract.States;
namespace DPStrategies;
// отдельно домен в один проект, в другой инфраструктуру, далее от задач
// domen  -философ и вилка
public class NaiveStrategy : IPhilosopherStrategy
{
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
    public bool Decide(Philosopher philosopher)
    {
        if (!philosopher.Step())
        {
            if (!philosopher.HasLeftFork())
            {
                if (philosopher.TryTakeLeftFork())
                {
                    _deadlockTime[philosopher] = 0;
                }
            }
            else
            {
                if (philosopher.TryTakeRightFork())
                {
                    _deadlockTime[philosopher] = 0;
                }
                else
                {
                    _deadlockTime[philosopher]++;
                }
            }
            if (_deadlockTime[philosopher] > 2)
            {
                return false;
            }
        }
        return true;
    }

}