using DiningPhilosophers.Contracts;

namespace DiningPhilosophers.Strategies.Coordinator;

// Дворецкий
// FIFO очередь.
public sealed class ButlerCoordinator : ICoordinator
{
    private readonly int _n;
    private readonly HashSet<int> _active = new();   // выдано разрешение
    private readonly Queue<int> _queue = new();      // ожидающие разрешения

    public ButlerCoordinator(int philosophersCount)
    {
        _n = philosophersCount;
    }
    
    public event Action<int, ForkSide>? AllowTakeFork;
    public event Action<int>? MustReleaseForks; 

    public void RequestToEat(int philosopherId)
    {
        if (_active.Contains(philosopherId) || _queue.Contains(philosopherId)) 
            return;

        if (_active.Count < _n - 1)
        {
            Grant(philosopherId);
        }
        else
        {
            _queue.Enqueue(philosopherId);
        }
    }

    public void SeatFreed(int philosopherId)
    {
        if (_active.Remove(philosopherId))
        {
            while (_active.Count < _n - 1 && _queue.Count > 0)
            {
                var next = _queue.Dequeue();
                if (_active.Contains(next)) 
                    continue;
                Grant(next);
            }
        }
    }

    private void Grant(int id)
    {
        _active.Add(id);
        AllowTakeFork?.Invoke(id, ForkSide.Left);
        AllowTakeFork?.Invoke(id, ForkSide.Right);
    }
}