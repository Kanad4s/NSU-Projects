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

        var simulation = new PDSimulation(appConfig);
        simulation.Simulate(appConfig.Simulation.Steps);
    }
}
