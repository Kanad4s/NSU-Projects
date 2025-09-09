using Microsoft.Extensions.Configuration;
using System;
using System.IO;
using Program.Simulation;


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
        foreach (var name in names.GetNames().ToArray())
        {
            Console.WriteLine(name);
        }

        var pd = PhilosopherFactory.CreatePhilosophers(names.GetNames());
        foreach (var pda in pd)
        {
            Console.WriteLine(pda._name);
        }
        
        
    }
}
