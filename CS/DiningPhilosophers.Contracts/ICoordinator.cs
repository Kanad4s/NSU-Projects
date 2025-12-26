namespace DiningPhilosophers.Contracts;

public interface ICoordinator
{
    void RequestToEat(int philosopherId);
    
    void SeatFreed(int philosopherId);

    event Action<int /*philosopherId*/, ForkSide /*side*/>? AllowTakeFork;

    event Action<int /*philosopherId*/>? MustReleaseForks;
}