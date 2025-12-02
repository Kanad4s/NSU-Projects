namespace DiningPhilosophers.App.GenericHost.Models;

public class PhilosopherMetrics
{
    public int Id { get; set; }
    public string Name { get; set; } = string.Empty;
    public int EatenCount { get; set; }
    public int HungryEpisodes { get; set; }
    public long TotalHungryMs { get; set; }
    public long MaxWaitingHungryMs { get; set; }
}
