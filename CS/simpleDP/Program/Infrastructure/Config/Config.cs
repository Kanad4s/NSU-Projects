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
}
