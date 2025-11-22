using System.Text;

namespace DiningPhilosophers.Simulator.Concurrent;

public interface IRealtimeRenderer
{
    void Render(
        long nowMs,
        IReadOnlyList<ConcurrentPhilosopherSnapshot> philosophers,
        IReadOnlyList<ConcurrentForkSnapshot> forks);
}

public sealed class RealtimeConsoleRenderer : IRealtimeRenderer
{
    public void Render(
        long nowMs,
        IReadOnlyList<ConcurrentPhilosopherSnapshot> philosophers,
        IReadOnlyList<ConcurrentForkSnapshot> forks)
    {
        var sb = new StringBuilder();
        sb.AppendLine($"===== t = {nowMs} ms =====");
        sb.AppendLine("Философы:");
        foreach (var p in philosophers)
        {
            sb.AppendLine($"  {p.Name}: {p.State}, has(L={p.HasLeftFork}, R={p.HasRightFork}), eaten={p.EatenCount}");
        }

        sb.AppendLine();
        sb.AppendLine("Вилки:");
        foreach (var f in forks)
        {
            var owner = f.OwnerId.HasValue
                ? $" (owner={f.OwnerId}{(f.InEatingUse ? ", eating" : ", blocked")})"
                : "";
            sb.AppendLine($"  Fork-{f.Id}: {f.State}{owner}");
        }

        Console.WriteLine(sb.ToString());
    }
}