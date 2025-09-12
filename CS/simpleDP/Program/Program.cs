using Microsoft.Extensions.Configuration;
using System;
using System.IO;
using Program.Simulation;
using DPStrategies;
using Program.Infrastructure.Config;


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

        var strategy = new NaiveStrategy();

        var philosophers = Factory.CreatePhilosophers([.. names.GetNames()], forks, appConfig);

        PDSimulation.Simulate(philosophers, forks, appConfig.Simulation.Steps, new NaiveStrategy());
    }
}
