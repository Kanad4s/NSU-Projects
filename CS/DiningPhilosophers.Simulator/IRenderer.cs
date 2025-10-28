namespace DiningPhilosophers.Simulator;

public interface IRenderer
{
    void RenderStep(int step, IReadOnlyList<PhilosopherSnapshot> philosophers, IReadOnlyList<ForkSnapshot> forks);
}