namespace DiningPhilosophers.Contracts;

public interface ICoordinator
{
    void RequestToEat(int philosopherId);
    
    // Философ закончил 
    void SeatFreed(int philosopherId);

    // Координатор разрешает взять конкретную вилку
    // event -- для инкапсуляции 
    // Action -- для полиморфизма, делегат 
    event Action<int /*philosopherId*/, ForkSide /*side*/>? AllowTakeFork;

    // Координатор требует освободить обе вилки
    event Action<int /*philosopherId*/>? MustReleaseForks;
}