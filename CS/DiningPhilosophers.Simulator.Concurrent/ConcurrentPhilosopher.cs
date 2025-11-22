using DiningPhilosophers.Contracts;
using System.Diagnostics;

namespace DiningPhilosophers.Simulator.Concurrent;

public sealed class ConcurrentPhilosopher
{
    private readonly Random _rand; 
    private readonly IPhilosopherStrategy _strategy;
    private readonly ConcurrentFork _left;
    private readonly ConcurrentFork _right;

    private volatile PhilosopherState _state = PhilosopherState.Thinking;

    private volatile bool _hasLeftFork;
    private volatile bool _hasRightFork;

    public int  Id { get; }
    public string Name { get; }
    public int  EatenCount { get; private set; }
    public long TotalHungryMs { get; private set; }
    public int  HungryEpisodes { get; private set; }
    public long MaxWaitingHungryMs { get; private set; }

    public ConcurrentPhilosopher(int id, string name, ConcurrentFork left, ConcurrentFork right, IPhilosopherStrategy strategy, int? seed = null)
    {
        Id = id;
        Name = name;
        _left = left;
        _right = right;
        _strategy = strategy;
        _rand = seed.HasValue ? new Random(seed.Value) : new Random(unchecked(Environment.TickCount * 31 + id));
    }

    // CancellationToken - флаг отмены, который может подать внешний код
    public async Task RunAsync(Func<long> nowMs, CancellationToken ct)
    {
        while (!ct.IsCancellationRequested)
        {
            _state = PhilosopherState.Thinking;
            var think = Next(RealTimeTiming.ThinkMinMs, RealTimeTiming.ThinkMaxMs);
            try
            {
                await Task.Delay(think, ct);
            }
            catch (TaskCanceledException)
            {
                break;
            }

            _state = PhilosopherState.Hungry;
            HungryEpisodes++;
            var hungryStart = nowMs();

            while (!_hasLeftFork || !_hasRightFork)
            {
                if (ct.IsCancellationRequested) break;

                var lp = new LocalPerception(
                    SelfId: Id,
                    SelfState: _state,
                    HasLeftFork: _hasLeftFork,
                    HasRightFork: _hasRightFork,
                    LeftForkId:  _left.Id,
                    LeftForkState:  _left.ReadState(),
                    RightForkId: _right.Id,
                    RightForkState: _right.ReadState()
                );

                var decision = _strategy.Decide(in lp);

                if (!_hasLeftFork && decision == ActionDecision.TakeLeftFork)
                {
                    var acquired = _left.TryAcquire(Id, nowMs());
                    if (acquired)
                    {
                        // взятие 20 мс
                        try { await Task.Delay(RealTimeTiming.TakeForkMs, ct); } catch (TaskCanceledException) { break; }
                        _hasLeftFork = true;
                    }
                }
                else if (!_hasRightFork && decision == ActionDecision.TakeRightFork)
                {
                    var acquired = _right.TryAcquire(Id, nowMs());
                    if (acquired)
                    {
                        try { await Task.Delay(RealTimeTiming.TakeForkMs, ct); } catch (TaskCanceledException) { break; }
                        _hasRightFork = true;
                    }
                }
                else if (decision == ActionDecision.ReleaseLeftFork && _hasLeftFork)
                {
                    _left.Release(nowMs());
                    _hasLeftFork = false;
                }
                else if (decision == ActionDecision.ReleaseRightFork && _hasRightFork)
                {
                    _right.Release(nowMs());
                    _hasRightFork = false;
                }

                if (_hasLeftFork && _hasRightFork)
                {
                    var waited = nowMs() - hungryStart;
                    TotalHungryMs += waited;
                    if (waited > MaxWaitingHungryMs) MaxWaitingHungryMs = waited;

                    _left.BeginEatingUse(Id, nowMs());
                    _right.BeginEatingUse(Id, nowMs());
                    break;
                }
            }

            if (!(_hasLeftFork && _hasRightFork)) break; // отмена во время Hungry

            _state = PhilosopherState.Eating;
            var eat = Next(RealTimeTiming.EatMinMs, RealTimeTiming.EatMaxMs);
            try { await Task.Delay(eat, ct); } catch (TaskCanceledException) { break; }
            EatenCount++;

            _left.Release(nowMs());
            _right.Release(nowMs());
            _hasLeftFork = _hasRightFork = false;

        }
    }

    private int Next(int minInclusive, int maxInclusive) =>
        _rand.Next(minInclusive, maxInclusive + 1);

    public ConcurrentPhilosopherSnapshot Snapshot(long now)
    {
        return new ConcurrentPhilosopherSnapshot(
            Id, Name, _state,
            _hasLeftFork, _hasRightFork,
            EatenCount
        );
    }
}

public readonly record struct ConcurrentPhilosopherSnapshot(
    int Id,
    string Name,
    PhilosopherState State,
    bool HasLeftFork,
    bool HasRightFork,
    int EatenCount
);
