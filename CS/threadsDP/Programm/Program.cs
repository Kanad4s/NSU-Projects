using DPContract;
using DPStrategy;
using Microsoft.Extensions.Configuration;
using Programm.Infrastructure.Config;
using Programm.Simulation;


namespace Programm;

public class Program
{
    static void Main(string[] args)
    {
        var appConfig = AppConfig.GetConfig();

        Console.WriteLine($"Steps: {appConfig.Simulation.Steps}");
        Console.WriteLine($"Philosophers file: {appConfig.PhilosophersFile}");

        var names = new FilePhilosopherNameProvider(appConfig.PhilosophersFile);

        var forks = Factory.CreateForks(names.GetNames().Count());

        var philosophers = Factory.CreatePhilosophers([.. names.GetNames()], forks, appConfig);

        // var strategy = new NaiveStrategy(philosophers);

        // var coordinator = new Coordinator(philosophers, forks);
        // var strategy = new CoordinatorStrategy(philosophers, coordinator);

        // var simulation = new PDSimulation(philosophers, forks, strategy);
        // simulation.Simulate(appConfig.Simulation.Steps);
    }
}
