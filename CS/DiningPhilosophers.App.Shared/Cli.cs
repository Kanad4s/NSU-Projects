using DiningPhilosophers.Contracts;
using DiningPhilosophers.Strategies.Coordinator;
using DiningPhilosophers.Strategies.Naive;

namespace DiningPhilosophers.App.Shared;

public static class Cli
{
    public static Dictionary<string, string> ParseArgs(string[] args)
    {
        var dict = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);
        for (int i = 0; i < args.Length; i++)
        {
            var a = args[i];
            if (a.StartsWith("--"))
            {
                var key = a[2..];
                string val = "true";
                if (i + 1 < args.Length && !args[i + 1].StartsWith("--"))
                {
                    val = args[i + 1];
                    i++;
                }
                dict[key] = val;
            }
        }
        return dict;
    }

    public static int GetInt(Dictionary<string,string> a, string key, int @default)
        => a.TryGetValue(key, out var s) && int.TryParse(s, out var v) ? v : @default;

    public static string GetString(Dictionary<string,string> a, string key, string @default)
        => a.TryGetValue(key, out var s) ? s : @default;
}

public static class StrategyFactory
{
    public static (IPhilosopherStrategy strategy, ICoordinator? coordinator) Create(string name, int philosophers)
    {
        switch (name.ToLowerInvariant())
        {
            case "naive":
                return (new NaiveLeftRightStrategy(), null);
            case "coordinated":
            case "butler":
                var coord = new ButlerCoordinator(philosophers);
                return (new CoordinatedStrategy(coord), coord);
            default:
                throw new ArgumentException($"Unknown strategy: {name}");
        }
    }
}

