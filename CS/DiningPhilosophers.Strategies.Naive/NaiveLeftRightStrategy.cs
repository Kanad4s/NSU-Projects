using DiningPhilosophers.Contracts;

namespace DiningPhilosophers.Strategies.Naive;


// в Hungry сначала пытаемся взять левую вилку,
// затем правую. Ничего не отпускаем
public sealed class NaiveLeftRightStrategy : IPhilosopherStrategy
{
    public ActionDecision Decide(in LocalPerception v)
    {
        if (v.SelfState != PhilosopherState.Hungry)
        {
            return ActionDecision.None;
        }
        
        if (!v.HasLeftFork && v.LeftForkState == ForkState.Available)
        {
            return ActionDecision.TakeLeftFork;
        }

        if (!v.HasRightFork && v.RightForkState == ForkState.Available)
        {
            return ActionDecision.TakeRightFork;
        }

        return ActionDecision.None;
    }
}