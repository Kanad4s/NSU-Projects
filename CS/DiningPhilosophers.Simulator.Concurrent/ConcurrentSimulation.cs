using System.Diagnostics;
using DiningPhilosophers.Contracts;

namespace DiningPhilosophers.Simulator.Concurrent;

public sealed class ConcurrentSimulation
{
    private readonly List<ConcurrentFork> _forks = new();
    private readonly List<ConcurrentPhilosopher> _philosophers = new();

    private readonly string[] _names;
    private readonly IPhilosopherStrategy _strategy;
    private readonly IRealtimeRenderer? _renderer;
    private readonly int? _seed;

    private CancellationTokenSource? _cts;
    private Task[]? _philosopherTasks;
    private Task? _renderTask;
    private Stopwatch? _sw;

    public ConcurrentSimulation(IReadOnlyList<string> names,
        IPhilosopherStrategy strategy,
        IRealtimeRenderer? renderer = null,
        int? seed = null)
    {
        _names = names.ToArray();
        _strategy = strategy;
        _renderer = renderer;
        _seed = seed;
    }
    
    public async Task RunAsync(
        int durationMs = RealTimeTiming.DurationMs,
        int renderPeriodMs = RealTimeTiming.RenderPeriodMs,
        CancellationToken cancellationToken = default)
    {
        Start(renderPeriodMs);
        try
        {
            using var linked = CancellationTokenSource.CreateLinkedTokenSource(_cts!.Token, cancellationToken);
            await Task.Delay(durationMs, linked.Token);
        }
        catch (TaskCanceledException) { }
        finally
        {
            await StopAsync();
        }
    }

    public void Start(int renderPeriodMs = RealTimeTiming.RenderPeriodMs)
    {
        if (_cts != null) throw new InvalidOperationException("Simulation already started.");

        _cts = new CancellationTokenSource(); // источник отмены
        _sw = Stopwatch.StartNew();

        // создаём вилки (lastChange=0, т.к. Stopwatch стартует с 0)
        for (int i = 0; i < _names.Length; i++)
            _forks.Add(new ConcurrentFork(i + 1, nowMs: 0));

        for (int i = 0; i < _names.Length; i++)
        {
            var left  = _forks[i];
            var right = _forks[(i + 1) % _names.Length];
            var p = new ConcurrentPhilosopher(
                id: i + 1,
                name: _names[i],
                left: left,
                right: right,
                strategy: _strategy,
                seed: _seed.HasValue ? _seed + i : null);
            _philosophers.Add(p);
        }

        var ct = _cts.Token;
        _philosopherTasks = _philosophers
            .Select(p => Task.Run(() => p.RunAsync(() => _sw!.ElapsedMilliseconds, ct), ct))
            .ToArray();


        if (_renderer != null)
        {
            _renderTask = Task.Run(async () =>
            {
                while (!ct.IsCancellationRequested)
                {
                    RenderTick();
                    try { await Task.Delay(renderPeriodMs, ct); } catch { break; }
                }
                RenderTick();
            }, ct);
        }
    }

    private void RenderTick()
    {
        if (_renderer == null || _sw == null) return;
        var now = _sw.ElapsedMilliseconds;

        var ph = _philosophers.Select(p => p.Snapshot(now)).ToList();
        var fk = _forks.Select(f => f.Snapshot(now)).ToList();

        _renderer.Render(now, ph, fk);
    }

    public async Task StopAsync()
    {
        if (_cts == null) return;

        // отмена всех задач
        _cts.Cancel();

        try
        {
            if (_philosopherTasks != null)
                await Task.WhenAll(_philosopherTasks);
        }
        catch (OperationCanceledException) { }

        if (_renderTask != null)
        {
            try { await _renderTask; } catch { }
        }

        _sw?.Stop();
    }

    public void PrintFinalReport()
    {
        // реально прошедшее время
        var elapsed = Math.Max(1, _sw?.ElapsedMilliseconds ?? 1);

        Console.WriteLine();
        Console.WriteLine("===== FINAL REPORT (Real-time) =====");
        Console.WriteLine($"Elapsed: {elapsed} ms");

        var totalEaten = _philosophers.Sum(p => p.EatenCount);
        Console.WriteLine($"Score (total eaten): {totalEaten}");

        Console.WriteLine();
        Console.WriteLine("Throughput (meals per ms):");
        double sumTp = 0;
        foreach (var p in _philosophers)
        {
            var tp = p.EatenCount / (double)elapsed;
            sumTp += tp;
            Console.WriteLine($"  {p.Name}: {tp:F6}");
        }
        Console.WriteLine($"  Average: {(sumTp / _philosophers.Count):F6}");

        Console.WriteLine();
        Console.WriteLine("Waiting time (Hungry), ms:");
        foreach (var p in _philosophers)
        {
            var avg = p.HungryEpisodes > 0
                ? p.TotalHungryMs / (double)p.HungryEpisodes
                : 0.0;
            Console.WriteLine($"  {p.Name}: avg={avg:F2}, max={p.MaxWaitingHungryMs}, episodes={p.HungryEpisodes}");
        }
        var worst = _philosophers.OrderByDescending(x => x.MaxWaitingHungryMs).FirstOrDefault();
        Console.WriteLine($"Max waiting overall: {worst.Name} = {worst.MaxWaitingHungryMs} ms");

        Console.WriteLine();
        Console.WriteLine("Fork utilization, %:");
        foreach (var f in _forks)
        {
            f.AccumulateToNow(_sw?.ElapsedMilliseconds ?? elapsed);

            double freePct    = 100.0 * f.FreeMs    / elapsed;
            double blockedPct = 100.0 * f.BlockedMs / elapsed;
            double eatingPct  = 100.0 * f.EatingMs  / elapsed;
            Console.WriteLine($"  Fork-{f.Id}: free={freePct:F1}% blocked={blockedPct:F1}% eating={eatingPct:F1}%");
        }
    }
}
