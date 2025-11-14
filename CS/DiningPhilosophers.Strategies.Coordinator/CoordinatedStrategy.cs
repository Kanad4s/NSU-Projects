using DiningPhilosophers.Contracts;

namespace DiningPhilosophers.Strategies.Coordinator;

/// <summary>
/// Стратегия берёт вилки только по разрешениям координатора.
/// </summary>
public sealed class CoordinatedStrategy : IPhilosopherStrategy
{
    private readonly ICoordinator _coordinator;

    private readonly Dictionary<int, Perm> _perm = new(); // флаги разершенный вилок
    private readonly HashSet<int> _active = new(); // философы, у которых сейчас занято место у координатора

    private readonly struct Perm
    {
        public readonly bool L, R;

        public Perm(bool l, bool r)
        {
            L=l; R=r;
        }
    }

    public CoordinatedStrategy(ICoordinator coordinator)
    {
        _coordinator = coordinator;

        _coordinator.AllowTakeFork += (id, side) =>
        {
            var p = _perm.TryGetValue(id, out var cur) ? cur : new Perm(false, false);
            _perm[id] = side == ForkSide.Left ? new Perm(true, p.R) : new Perm(p.L, true);
            _active.Add(id);
        };
    }

    public ActionDecision Decide(in LocalPerception v)
    {
        // Если философ уже Thinking, но числится "активным" у координатора — освобождаем слот
        if (v.SelfState == PhilosopherState.Thinking && _active.Contains(v.SelfId))
        {
            _active.Remove(v.SelfId);
            _perm.Remove(v.SelfId);
            _coordinator.SeatFreed(v.SelfId);
            return ActionDecision.None;
        }

        // Если не Hungry — стратегия ничего не делает
        if (v.SelfState != PhilosopherState.Hungry)
            return ActionDecision.None;

        // Если обе вилки уже в руках — координатор больше не нужен (философ скоро передет в Eating)
        if (v.HasLeftFork && v.HasRightFork)
        {
            _perm.Remove(v.SelfId);
            return ActionDecision.None;
        }

        var hasPerm = _perm.GetValueOrDefault(v.SelfId);

        // по разрешению координатора и если вилка свободна — пробуем её взять
        if (!v.HasLeftFork  && hasPerm.L && v.LeftForkState  == ForkState.Available)  return ActionDecision.TakeLeftFork;
        if (!v.HasRightFork && hasPerm.R && v.RightForkState == ForkState.Available)  return ActionDecision.TakeRightFork;

        return ActionDecision.None;
    }
}
