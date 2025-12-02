using System.Diagnostics;
using DiningPhilosophers.App.GenericHost.Infrastructure;
using DiningPhilosophers.App.GenericHost.Models;
using DiningPhilosophers.Contracts;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;
using Microsoft.Extensions.Options;

namespace DiningPhilosophers.App.GenericHost.Services;


public class PhilosopherHostedService : BackgroundService
{
    private readonly int _id;
    private readonly string _name;
    private readonly IPhilosopherStrategy _strategy;
    private readonly ITableManager _tableManager;
    private readonly IMetricsCollector _metricsCollector;
    private readonly SimulationOptions _options;
    private readonly ILogger<PhilosopherHostedService> _logger;
    private readonly Random _random;

    private PhilosopherState _state = PhilosopherState.Thinking;
    private bool _hasLeftFork;
    private bool _hasRightFork;

    public PhilosopherHostedService(
        int id,
        string name,
        IPhilosopherStrategy strategy,
        ITableManager tableManager,
        IMetricsCollector metricsCollector,
        IOptions<SimulationOptions> options,
        ILogger<PhilosopherHostedService> logger)
    {
        _id = id;
        _name = name;
        _strategy = strategy;
        _tableManager = tableManager;
        _metricsCollector = metricsCollector;
        _options = options.Value;
        _logger = logger;
        _random = new Random(unchecked(Environment.TickCount * 31 + id));
    }

    protected override async Task ExecuteAsync(CancellationToken stoppingToken)
    {
        _logger.LogInformation("Философ {Name} (#{Id}) начал работу", _name, _id);

        try
        {
            while (!stoppingToken.IsCancellationRequested)
            {
                await ThinkAsync(stoppingToken);

                if (stoppingToken.IsCancellationRequested)
                    break;

                await TryToEatAsync(stoppingToken);
            }
        }
        catch (OperationCanceledException)
        {
            // Нормальное завершение
            _logger.LogInformation("Философ {Name} (#{Id}) получил сигнал отмены", _name, _id);
        }
        catch (Exception ex)
        {
            _logger.LogError(ex, "Философ {Name} (#{Id}) завершился с ошибкой", _name, _id);
        }
        finally
        {
            ReleaseForks();
            _logger.LogInformation("Философ {Name} (#{Id}) завершил работу", _name, _id);
        }
    }

    private async Task ThinkAsync(CancellationToken ct)
    {
        _state = PhilosopherState.Thinking;
        var thinkTime = _random.Next(_options.ThinkingTimeMin, _options.ThinkingTimeMax + 1);
        
        _logger.LogDebug("Философ {Name} размышляет {Time}мс", _name, thinkTime);
        
        await Task.Delay(thinkTime, ct);
    }

    private async Task TryToEatAsync(CancellationToken ct)
    {
        _state = PhilosopherState.Hungry;
        _metricsCollector.RecordHungryEpisode(_id);
        
        var hungryStart = Stopwatch.GetTimestamp();
        
        _logger.LogDebug("Философ {Name} голоден", _name);

        while (!_hasLeftFork || !_hasRightFork)
        {
            if (ct.IsCancellationRequested)
                break;

            var leftForkId = _tableManager.GetLeftForkId(_id);
            var rightForkId = _tableManager.GetRightForkId(_id);

            var perception = new LocalPerception(
                SelfId: _id,
                SelfState: _state,
                HasLeftFork: _hasLeftFork,
                HasRightFork: _hasRightFork,
                LeftForkId: leftForkId,
                LeftForkState: _tableManager.GetForkState(leftForkId),
                RightForkId: rightForkId,
                RightForkState: _tableManager.GetForkState(rightForkId)
            );

            var decision = _strategy.Decide(in perception);

            switch (decision)
            {
                case ActionDecision.TakeLeftFork when !_hasLeftFork:
                    if (_tableManager.TryAcquireFork(_id, leftForkId))
                    {
                        await Task.Delay(_options.ForkAcquisitionTime, ct);
                        _hasLeftFork = true;
                        _logger.LogDebug("Философ {Name} взял левую вилку #{Fork}", _name, leftForkId);
                    }
                    break;

                case ActionDecision.TakeRightFork when !_hasRightFork:
                    if (_tableManager.TryAcquireFork(_id, rightForkId))
                    {
                        await Task.Delay(_options.ForkAcquisitionTime, ct);
                        _hasRightFork = true;
                        _logger.LogDebug("Философ {Name} взял правую вилку #{Fork}", _name, rightForkId);
                    }
                    break;

                case ActionDecision.ReleaseLeftFork when _hasLeftFork:
                    _tableManager.ReleaseFork(leftForkId);
                    _hasLeftFork = false;
                    _logger.LogDebug("Философ {Name} освободил левую вилку #{Fork}", _name, leftForkId);
                    break;

                case ActionDecision.ReleaseRightFork when _hasRightFork:
                    _tableManager.ReleaseFork(rightForkId);
                    _hasRightFork = false;
                    _logger.LogDebug("Философ {Name} освободил правую вилку #{Fork}", _name, rightForkId);
                    break;
            }

            if (!_hasLeftFork || !_hasRightFork)
            {
                await Task.Delay(10, ct);
            }
        }

        if (_hasLeftFork && _hasRightFork)
        {
            _state = PhilosopherState.Eating;
            var waitTimeMs = (long)((Stopwatch.GetTimestamp() - hungryStart) / (Stopwatch.Frequency / 1000.0));
            _metricsCollector.RecordEatingSession(_id, waitTimeMs);

            var eatTime = _random.Next(_options.EatingTimeMin, _options.EatingTimeMax + 1);
            _logger.LogDebug("Философ {Name} ест {Time}мс (ждал {Wait}мс)", _name, eatTime, waitTimeMs);

            await Task.Delay(eatTime, ct);

            ReleaseForks();
        }
    }

    private void ReleaseForks()
    {
        if (_hasLeftFork)
        {
            _tableManager.ReleaseFork(_tableManager.GetLeftForkId(_id));
            _hasLeftFork = false;
        }
        if (_hasRightFork)
        {
            _tableManager.ReleaseFork(_tableManager.GetRightForkId(_id));
            _hasRightFork = false;
        }
    }
}
