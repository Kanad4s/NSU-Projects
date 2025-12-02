using DiningPhilosophers.App.GenericHost.Infrastructure;
using DiningPhilosophers.App.GenericHost.Models;
using DiningPhilosophers.App.GenericHost.Services;
using DiningPhilosophers.App.Shared;
using DiningPhilosophers.Contracts;
using DiningPhilosophers.Strategies.Coordinator;
using DiningPhilosophers.Strategies.Naive;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;

var builder = Host.CreateApplicationBuilder(args);

builder.Configuration
    .AddJsonFile("appsettings.json", optional: false, reloadOnChange: true)
    .AddCommandLine(args);

builder.Services.Configure<SimulationOptions>(
    builder.Configuration.GetSection("Simulation"));

var names = Names.ReadNamesOrDefault("names.txt");
var philosopherCount = names.Count;

builder.Services.AddSingleton<ITableManager>(_ => new TableManager(philosopherCount));
builder.Services.AddSingleton<IMetricsCollector>(_ =>
{
    var collector = new MetricsCollector();
    for (int i = 0; i < names.Count; i++)
    {
        collector.RegisterPhilosopher(i + 1, names[i]);
    }
    return collector;
});

var strategyName = builder.Configuration["Strategy"] ?? "naive";

ICoordinator? coordinator = null;
if (strategyName.ToLowerInvariant() is "coordinator" or "butler")
{
    coordinator = new ButlerCoordinator(philosopherCount);
    builder.Services.AddSingleton(coordinator);
}

builder.Services.AddSingleton<IPhilosopherStrategy>(_ =>
{
    return strategyName.ToLowerInvariant() switch
    {
        "coordinator" or "butler" => new CoordinatedStrategy(coordinator!),
        _ => new NaiveLeftRightStrategy()
    };
});

for (int i = 0; i < names.Count; i++)
{
    int philosopherId = i + 1;
    string philosopherName = names[i];
    
    builder.Services.AddSingleton<IHostedService>(sp =>
    {
        var strategy = sp.GetRequiredService<IPhilosopherStrategy>();
        var tableManager = sp.GetRequiredService<ITableManager>();
        var metricsCollector = sp.GetRequiredService<IMetricsCollector>();
        var options = sp.GetRequiredService<Microsoft.Extensions.Options.IOptions<SimulationOptions>>();
        var logger = sp.GetRequiredService<ILogger<PhilosopherHostedService>>();

        return new PhilosopherHostedService(
            philosopherId,
            philosopherName,
            strategy,
            tableManager,
            metricsCollector,
            options,
            logger);
    });
}

builder.Services.AddHostedService<SimulationLifetimeService>();

var host = builder.Build();

Console.WriteLine("СИМУЛЯЦИЯ ОБЕДАЮЩИХ ФИЛОСОФОВ (.NET Generic Host)");
Console.WriteLine($"\nФилософы: {string.Join(", ", names)}");
Console.WriteLine($"Стратегия: {strategyName}");
Console.WriteLine($"Длительность: {builder.Configuration["Simulation:DurationSeconds"]} секунд\n");
Console.WriteLine("Нажмите Ctrl+C для преждевременной остановки...\n");

await host.RunAsync();
