using DPStrategyContract;

namespace DPStrategies;

public class Coordinator
{
    private List<Philosopher> _philosophers;
    private List<Fork> _forksInProcess;

    private List<Philosopher> _hungryQueue;
    private List<Philosopher> _processingHungryQueue;

    public event TryTakeFork TakeFork;

    public Coordinator(List<Philosopher> philosophers, List<Fork> forks)
    {
        _philosophers = philosophers;
        _forksInProcess = [];
        _hungryQueue = [];
        _processingHungryQueue = [];
    }

    public void TakeHungry(Philosopher philosopher)
    {
        if (_processingHungryQueue.Contains(philosopher))
        {
            return;
        }
        else if (_hungryQueue.Contains(philosopher))
        {
            return;
        }

        _hungryQueue.Add(philosopher);
    }

    // Философ изъявляет желание поесть в последний ход раздумий, поэтому только на следующем ходу сможет получить вилку
    // Т.е. каждое действие занимает ход
    public int DecideHungry()
    {
        int decideCount = 0;
        foreach (var p in _processingHungryQueue.ToList())
        {
            if (TakeFork(p, GetFork(p)))
            {
                decideCount++;
            }
            if (p.HasLeftFork() && p.HasRightFork())
            {
                FinishProcessPhilosopher(p);
            }
        }
        foreach (var p in _hungryQueue.ToList())
        {
            if (IsAbleToProcess(p))
            {
                StartProcessPhilosopher(p);
            }
        }
        return decideCount;
    }

    private void StartProcessPhilosopher(Philosopher philosopher)
    {
        _forksInProcess.Add(philosopher.LeftFork);
        _forksInProcess.Add(philosopher.RightFork);
        _processingHungryQueue.Add(philosopher);
        _hungryQueue.Remove(philosopher);
    }

    private void FinishProcessPhilosopher(Philosopher philosopher)
    {
        _processingHungryQueue.Remove(philosopher);
        _forksInProcess.Remove(philosopher.LeftFork);
        _forksInProcess.Remove(philosopher.RightFork);
    }

    private bool IsAbleToProcess(Philosopher philosopher)
    {
        if (philosopher.LeftFork.IsAvailable() && philosopher.RightFork.IsAvailable())
        {
            if (!_forksInProcess.Contains(philosopher.LeftFork) && !_forksInProcess.Contains(philosopher.RightFork))
            {
                return true;
            }
        }
        return false;
    }

    private Fork GetFork(Philosopher philosopher)
    {
        if (!philosopher.HasLeftFork())
        {
            return philosopher.LeftFork;
        }
        return philosopher.RightFork;
    }

    private bool AreForksAvailable(Philosopher philosopher)
    {
        if (!philosopher.HasLeftFork() && !philosopher.HasRightFork())
        {
            if (!_forksInProcess.Contains(philosopher.LeftFork) && !_forksInProcess.Contains(philosopher.RightFork))
            {
                return true;
            }
        }
        return false;
    }
}

public delegate bool TakeFork(Philosopher philosopher, Fork fork);