using System.IO;
using Microsoft.Extensions.Configuration;

namespace Programm.Infrastructure.Config
{
    public class SimulationConfig
    {
        public int Steps { get; set; }
        public int ThinkingMin { get; set; }
        public int ThinkingMax { get; set; }
        public int EatingMin { get; set; }
        public int EatingMax { get; set; }
    }

    public class AppConfig
    {
        public SimulationConfig Simulation { get; set; } = new();
        public string PhilosophersFile { get; set; } = "Config/appsettings.json";

        public static AppConfig GetConfig()
        {
            var builder = new ConfigurationBuilder()
                .SetBasePath(Directory.GetCurrentDirectory())
                .AddJsonFile("Config/appsettings.json", optional: false, reloadOnChange: true);

            IConfiguration configuration = builder.Build();

            return configuration.Get<AppConfig>() ?? new AppConfig();
        }

    }

}