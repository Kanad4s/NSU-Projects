namespace DiningPhilosophers.Simulator.Concurrent;


public static class RealTimeTiming
{
    public const int DurationMs = 15_000;

    public const int RenderPeriodMs = 150;    

    public const int ThinkMinMs = 30;
    public const int ThinkMaxMs = 100;

    public const int EatMinMs = 40;
    public const int EatMaxMs = 50;

    public const int TakeForkMs = 20;
}