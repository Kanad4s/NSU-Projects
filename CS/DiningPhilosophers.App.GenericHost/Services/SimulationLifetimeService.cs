using DiningPhilosophers.App.GenericHost.Infrastructure;
using DiningPhilosophers.App.GenericHost.Models;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;
using Microsoft.Extensions.Options;

namespace DiningPhilosophers.App.GenericHost.Services;

public class SimulationLifetimeService : BackgroundService
{
    private readonly IHostApplicationLifetime _lifetime;
    private readonly IMetricsCollector _metricsCollector;
    private readonly SimulationOptions _options;
    private readonly ILogger<SimulationLifetimeService> _logger;

    public SimulationLifetimeService(
        IHostApplicationLifetime lifetime,
        IMetricsCollector metricsCollector,
        IOptions<SimulationOptions> options,
        ILogger<SimulationLifetimeService> logger)
    {
        _lifetime = lifetime;
        _metricsCollector = metricsCollector;
        _options = options.Value;
        _logger = logger;
    }

    protected override async Task ExecuteAsync(CancellationToken stoppingToken)
    {
        _logger.LogInformation("Симуляция запущена на {Duration} секунд", _options.DurationSeconds);

        try
        {
            await Task.Delay(TimeSpan.FromSeconds(_options.DurationSeconds), stoppingToken);
        }
        catch (OperationCanceledException)
        {
            // Симуляция прервана извне
            _logger.LogInformation("Симуляция прервана");
        }

        _logger.LogInformation("Время симуляции истекло. Завершение работы...");
        
        _lifetime.StopApplication();
    }

    public override async Task StopAsync(CancellationToken cancellationToken)
    {
        _logger.LogInformation("Остановка симуляции...");
        
        await base.StopAsync(cancellationToken);
        
        _metricsCollector.PrintFinalReport();
    }
}
