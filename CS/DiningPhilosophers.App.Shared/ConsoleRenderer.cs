using System.Text;
using DiningPhilosophers.Contracts;
using DiningPhilosophers.Simulator;

namespace DiningPhilosophers.App.Shared;

public sealed class ConsoleRenderer : IRenderer
{
    public void RenderStep(int step, IReadOnlyList<PhilosopherSnapshot> philosophers, IReadOnlyList<ForkSnapshot> forks)
    {
        var sb = new StringBuilder();
        sb.AppendLine($"===== ШАГ {step} =====");
        sb.AppendLine("Философы:");
        foreach (var p in philosophers)
        {
            var action = p.ActionHint is null ? "" : $" (Action = {p.ActionHint})";
            var extra = p.State switch
            {
                PhilosopherState.Eating => $" ({p.EatingStepsLeft} steps left)",
                PhilosopherState.Thinking => p.ThinkingStepsLeft.HasValue ? $" ({p.ThinkingStepsLeft} steps left)" : "",
                _ => ""
            };
            sb.AppendLine($"  {p.Name}: {p.State}{extra}{action}, съедено: {p.EatenCount}");
        }
        sb.AppendLine();
        sb.AppendLine("Вилки:");
        var nameById = philosophers.ToDictionary(p => p.Id, p => p.Name);
        foreach (var f in forks)
        {
            var who = f.OwnerPhilosopherId.HasValue && nameById.TryGetValue(f.OwnerPhilosopherId.Value, out var ownerName)
                ? $" (используется {ownerName})"
                : "";
            sb.AppendLine($"  Fork-{f.Id}: {f.State}{who}");
        }
        Console.WriteLine(sb.ToString());
    }
}
