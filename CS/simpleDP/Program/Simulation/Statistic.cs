using DPStrategyContract;
using DPStrategyContract.States;

namespace Program.Simulation;
public class Statistic
{
    public Dictionary<string, int> PhilosopherWaiting = [];
    public Dictionary<string, int> MealsEaten = [];
    public Dictionary<int, int> ForkAvailable = [];
    public Dictionary<int, int> ForkBlocked = [];

    public int Steps = 0;

    public Statistic(List<Philosopher> philosophers, List<Fork> forks)
    {
        foreach (var p in philosophers)
        {
            PhilosopherWaiting.Add(p.Name, 0);
            MealsEaten.Add(p.Name, 0);

        }

        foreach (var f in forks)
        {
            ForkAvailable.Add(f.Id, 0);
            ForkBlocked.Add(f.Id, 0);
        }
    }


    public void StepUpdate(int step, List<Philosopher> philosophers, List<Fork> forks)
    {
        foreach (var p in philosophers)
        {
            if (p.State == PhilosopherState.Hungry)
            {
                PhilosopherWaiting[p.Name]++;
            }
            MealsEaten[p.Name] = p.MealsEaten;
        }

        foreach (var f in forks)
        {
            if (f.IsInUse())
            {
                if (f.Owner?.State == PhilosopherState.Hungry)
                {
                    ForkBlocked[f.Id]++;
                }
            }
            else
            {
                ForkAvailable[f.Id]++;
            }
        }
        Steps++;
    }
}