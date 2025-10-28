using DiningPhilosophers.Contracts;

namespace DiningPhilosophers.Strategies.Naive;

// Философ всегда берет вилку с меньшим ID первой, затем вилку с большим ID.
public sealed class ResourceHierarchyStrategy : IPhilosopherStrategy
{
    public ActionDecision Decide(in LocalPerception v)
    {
        if (v.SelfState != PhilosopherState.Hungry) 
            return ActionDecision.None;

        var leftId = v.LeftForkId;
        var rightId = v.RightForkId;

        var lowerForkIsLeft = leftId < rightId;

        if (lowerForkIsLeft)
        {
            if (!v.HasLeftFork && v.LeftForkState == ForkState.Available)
            {
                return ActionDecision.TakeLeftFork;
            }

            if (v.HasLeftFork && !v.HasRightFork && v.RightForkState == ForkState.Available)
            {
                return ActionDecision.TakeRightFork;
            }
        }
        else
        {
            if (!v.HasRightFork && v.RightForkState == ForkState.Available)
            {
                return ActionDecision.TakeRightFork;
            }

            if (v.HasRightFork && !v.HasLeftFork && v.LeftForkState == ForkState.Available)
            {
                return ActionDecision.TakeLeftFork;
            }
        }

        return ActionDecision.None;
    }
}

