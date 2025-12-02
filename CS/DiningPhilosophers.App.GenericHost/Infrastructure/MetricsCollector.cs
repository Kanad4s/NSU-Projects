using System.Collections.Concurrent;
using DiningPhilosophers.App.GenericHost.Models;

namespace DiningPhilosophers.App.GenericHost.Infrastructure;

public class MetricsCollector : IMetricsCollector
{
    private readonly ConcurrentDictionary<int, PhilosopherMetrics> _metrics = new();
    
    public void RegisterPhilosopher(int id, string name)
    {
        _metrics.TryAdd(id, new PhilosopherMetrics 
        { 
            Id = id, 
            Name = name 
        });
    }

    public void RecordEatingSession(int philosopherId, long waitTimeMs)
    {
        if (_metrics.TryGetValue(philosopherId, out var metrics))
        {
            lock (metrics)
            {
                metrics.EatenCount++;
                metrics.TotalHungryMs += waitTimeMs;
                if (waitTimeMs > metrics.MaxWaitingHungryMs)
                {
                    metrics.MaxWaitingHungryMs = waitTimeMs;
                }
            }
        }
    }

    public void RecordHungryEpisode(int philosopherId)
    {
        if (_metrics.TryGetValue(philosopherId, out var metrics))
        {
            lock (metrics)
            {
                metrics.HungryEpisodes++;
            }
        }
    }

    public PhilosopherMetrics GetMetrics(int philosopherId)
    {
        return _metrics.GetValueOrDefault(philosopherId) ?? new PhilosopherMetrics();
    }

    public IReadOnlyList<PhilosopherMetrics> GetAllMetrics()
    {
        return _metrics.Values.OrderBy(m => m.Id).ToList();
    }

    public void PrintFinalReport()
    {
        Console.WriteLine("ФИНАЛЬНЫЙ ОТЧЁТ СИМУЛЯЦИИ");

        var allMetrics = GetAllMetrics();
        
        foreach (var m in allMetrics)
        {
            var avgWait = m.EatenCount > 0 ? m.TotalHungryMs / (double)m.EatenCount : 0;
            
            Console.WriteLine($"Философ #{m.Id} ({m.Name}):");
            Console.WriteLine($"  ├─ Съедено: {m.EatenCount}");
            Console.WriteLine($"  ├─ Эпизодов голода: {m.HungryEpisodes}");
            Console.WriteLine($"  ├─ Среднее ожидание: {avgWait:F1} мс");
            Console.WriteLine($"  └─ Максимальное ожидание: {m.MaxWaitingHungryMs} мс");
            Console.WriteLine();
        }

        var totalEaten = allMetrics.Sum(m => m.EatenCount);
        var totalHungry = allMetrics.Sum(m => m.HungryEpisodes);
        var avgEaten = allMetrics.Average(m => m.EatenCount);
        
        Console.WriteLine("---------------------------------------------------------");
        Console.WriteLine($"Всего съедено: {totalEaten} | Всего эпизодов голода: {totalHungry}");
        Console.WriteLine($"Среднее количество приёмов пищи на философа: {avgEaten:F1}");
    }
}
