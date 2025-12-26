namespace DiningPhilosophers.Simulator.Concurrent;


public static class RealTimeTiming
{
    public const int DurationMs = 15_000;

    public const int RenderPeriodMs = 150;    

    public const int ThinkMinMs = 120;
    public const int ThinkMaxMs = 200;

    public const int EatMinMs = 10;
    public const int EatMaxMs = 20;

    public const int TakeForkMs = 20;
}