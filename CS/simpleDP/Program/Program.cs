using Microsoft.Extensions.Configuration;
using Program.Simulation;
using DPStrategies;
using Program.Infrastructure.Config;
using DPStrategyContract;


namespace Program;

public class Program
{
    static void Main(string[] args)
    {
        var builder = new ConfigurationBuilder()
            .SetBasePath(Directory.GetCurrentDirectory())
            .AddJsonFile("Config/appsettings.json", optional: false, reloadOnChange: true);

        IConfiguration configuration = builder.Build();

        var appConfig = configuration.Get<AppConfig>() ?? new AppConfig();

        Console.WriteLine($"Steps: {appConfig.Simulation.Steps}");
        Console.WriteLine($"Philosophers file: {appConfig.PhilosophersFile}");

        var names = new FilePhilosopherNameProvider(appConfig.PhilosophersFile);

        var forks = Factory.CreateForks(names.GetNames().Count());

        var philosophers = Factory.CreatePhilosophers([.. names.GetNames()], forks, appConfig);

        // var strategy = new NaiveStrategy(philosophers);

        var coordinator = new Coordinator(philosophers, forks);
        var strategy = new CoordinatorStrategy(philosophers, coordinator);

        var simulation = new PDSimulation(philosophers, forks, strategy);
        simulation.Simulate(appConfig.Simulation.Steps);
    }
}
