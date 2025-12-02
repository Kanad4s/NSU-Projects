using DiningPhilosophers.App.GenericHost.Models;

namespace DiningPhilosophers.App.GenericHost.Infrastructure;

public interface IMetricsCollector
{
    void RecordEatingSession(int philosopherId, long waitTimeMs);
    void RecordHungryEpisode(int philosopherId);
    PhilosopherMetrics GetMetrics(int philosopherId);
    IReadOnlyList<PhilosopherMetrics> GetAllMetrics();
    void PrintFinalReport();
}

