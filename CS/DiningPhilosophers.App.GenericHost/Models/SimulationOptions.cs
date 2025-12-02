namespace DiningPhilosophers.App.GenericHost.Models;

public class SimulationOptions
{
    public int DurationSeconds { get; set; }
    public int ThinkingTimeMin { get; set; }
    public int ThinkingTimeMax { get; set; }
    public int EatingTimeMin { get; set; }
    public int EatingTimeMax { get; set; }
    public int ForkAcquisitionTime { get; set; }
    public int DisplayUpdateInterval { get; set; }
}

