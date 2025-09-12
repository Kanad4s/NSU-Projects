using DPStrategyContract;
using DPStrategyContract.Actions;
using DPStrategyContract.States;
namespace DPStrategies;
// отдельно домен в один проект, в другой инфраструктуру, далее от задач
// domen  -философ и вилка
public class NaiveStrategy() : IPhilosopherStrategy
{
    public string StrategyName { get; set; } = "Naive Strategy";
    public void Decide(Philosopher philosopher)
    {
        if (philosopher.Step())
        {
            return;
        }

        
    }

}