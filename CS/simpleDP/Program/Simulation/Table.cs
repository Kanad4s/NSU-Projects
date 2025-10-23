using DPStrategyContract;

namespace Program.Simulation;

// c кординатором и без
public class Table
{
    private int _seatsCount;
    private List<Philosopher> _philosophers;
    private List<Fork> _forks;

    public Table(List<Fork> forks)
    {
        _seatsCount = forks.Count;
        _philosophers = [];
        _forks = forks;
    }

    public bool SitPhilosophers(List<Philosopher> philosophers)
    {
        if (_philosophers.Count != 0)
        {
            Console.WriteLine("Table is not free");
            return false;
        }
        if (philosophers.Count > _seatsCount)
        {
            Console.WriteLine("Philosophers more then seats");
            return false;
        }
        else if (philosophers.Count < _seatsCount)
        {
            Console.WriteLine("Philosophers less then seats");
            return false;
        }
        _philosophers = philosophers;
        return true;
    }

    public bool SitPhilosopher(Philosopher philosopher)
    {
        if (_philosophers.Count < _seatsCount)
        {
            if (!_philosophers.Contains(philosopher))
            {
                _philosophers.Add(philosopher);
            }
            return true;
        }
        else if (_philosophers.Count == _seatsCount)
        {
            return _philosophers.Contains(philosopher);
        }
        return false;
    }

    // public bool SitFork(Fork fork)
    // {
    //     if (_forks.Count < _seatsCount)
    //     {
    //         if (!_forks.Contains(fork))
    //         {
    //             _forks.Add(fork);
    //         }
    //         return true;
    //     }
    //     else if (_forks.Count == _seatsCount)
    //     {
    //         return _forks.Contains(fork);
    //     }
    //     return false;
    // }
}