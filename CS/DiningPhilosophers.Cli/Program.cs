using DiningPhilosophers.App.Shared;
using DiningPhilosophers.Simulator;
using DiningPhilosophers.Simulator.Concurrent;

var argsMap = Cli.ParseArgs(args);
var mode = Cli.GetString(argsMap, "mode", "step"); 
var namesPath = Cli.GetString(argsMap, "names", "names.txt");
var strategyKey = Cli.GetString(argsMap, "strategy", "naive");
var seed = argsMap.TryGetValue("seed", out var seedStr) && int.TryParse(seedStr, out var s) ? s : (int?)null;

var names = Names.ReadNamesOrDefault(namesPath);

switch (mode.ToLowerInvariant())
{
    case "step":
    {
        var steps = Cli.GetInt(argsMap, "steps", 1_000_000);
        var renderEvery = Cli.GetInt(argsMap, "render-every", 1000);
        var (strategy, coordinator) = StrategyFactory.Create(strategyKey, names.Count);
        var sim = new Simulation(names, rng: seed.HasValue ? new DefaultRng(seed) : null, renderer: new ConsoleRenderer());
        sim.Run(strategy, steps, renderEvery, coordinator);
        sim.PrintFinalReport();
        break;
    }
    case "rt":
    case "realtime":
    {
        var duration = Cli.GetInt(argsMap, "duration", RealTimeTiming.DurationMs);
        var renderPeriod = Cli.GetInt(argsMap, "render-period", RealTimeTiming.RenderPeriodMs);
        var (strategy, _) = StrategyFactory.Create(strategyKey, names.Count);
        var renderer = new RealtimeConsoleRenderer();
        var sim = new ConcurrentSimulation(names, strategy, renderer, seed);
        await sim.RunAsync(duration, renderPeriod);
        sim.PrintFinalReport();
        break;
    }
    default:
        Console.Error.WriteLine($"Unknown --mode: {mode}. Use 'step' or 'rt'.");
        Environment.ExitCode = 2;
        break;
}
