using DiningPhilosophers.Contracts;

namespace DiningPhilosophers.Simulator;

public sealed class Philosopher
{
    public int Id { get; }
    public string Name { get; }
    public PhilosopherState State { get; private set; } = PhilosopherState.Thinking;

    public bool HasLeftFork { get; private set; }
    public bool HasRightFork { get; private set; }

    // Таймеры взятия вилок 
    private int _takingLeftStepsLeft;
    private int _takingRightStepsLeft;

    // Таймеры состояний
    private int _thinkingStepsLeft;
    private int _eatingStepsLeft;

    // Для метрики
    public int EatenCount { get; private set; }
    public int WaitingInHungry { get; private set; } 
    public int MaxWaitingInHungry { get; private set; }
    public string? LastActionHint { get; private set; }
    public long TotalHungrySteps { get; private set; }   
    public int  HungryEpisodes   { get; private set; } 
    
    private readonly Fork _leftFork;
    private readonly Fork _rightFork;
    private readonly DefaultRng _rng;

    public Philosopher(int id, string name, Fork leftFork, Fork rightFork, DefaultRng rng)
    {
        Id = id;
        Name = name;
        _leftFork = leftFork;
        _rightFork = rightFork;
        _rng = rng;
        _thinkingStepsLeft = _rng.NextInclusive(Timing.ThinkMin, Timing.ThinkMax);
    }

    public LocalPerception BuildView() => new(
        SelfId: Id,
        SelfState: State,
        HasLeftFork: HasLeftFork,
        HasRightFork: HasRightFork,
        LeftForkId: _leftFork.Id,
        LeftForkState: _leftFork.State,
        RightForkId: _rightFork.Id,
        RightForkState: _rightFork.State);

    public PhilosopherSnapshot Snapshot() => new(
        Id, Name, State, HasLeftFork, HasRightFork,
        EatingStepsLeft: State == PhilosopherState.Eating ? _eatingStepsLeft : null,
        ThinkingStepsLeft: State == PhilosopherState.Thinking ? _thinkingStepsLeft : null,
        ActionHint: LastActionHint,
        EatenCount,
        WaitingInHungry);

    public void TickBeforeDecision()
    {
        LastActionHint = null;

        // Идёт взятие вилки
        if (_takingLeftStepsLeft > 0)
        {
            _takingLeftStepsLeft--;
            if (_takingLeftStepsLeft == 0) HasLeftFork = true;
        }
        if (_takingRightStepsLeft > 0)
        {
            _takingRightStepsLeft--;
            if (_takingRightStepsLeft == 0) HasRightFork = true;
        }

        // Обработка текущего состояния
        switch (State)
        {
            case PhilosopherState.Thinking:
                if (_thinkingStepsLeft > 0) _thinkingStepsLeft--;
                if (_thinkingStepsLeft == 0)
                {
                    State = PhilosopherState.Hungry;
                    WaitingInHungry = 0;
                    HungryEpisodes++;
                }
                break;

            case PhilosopherState.Hungry:
                WaitingInHungry++;
                if (WaitingInHungry > MaxWaitingInHungry)
                {
                    MaxWaitingInHungry = WaitingInHungry;
                }
                break;

            case PhilosopherState.Eating:
                if (_eatingStepsLeft > 0) _eatingStepsLeft--;
                if (_eatingStepsLeft == 0)
                {
                    if (HasLeftFork) _leftFork.Release();
                    if (HasRightFork) _rightFork.Release();
                    HasLeftFork = HasRightFork = false;

                    State = PhilosopherState.Thinking;
                    _thinkingStepsLeft = _rng.NextInclusive(Timing.ThinkMin, Timing.ThinkMax);
                }
                break;
        }
    }

    public void DecideAndAct(IPhilosopherStrategy strategy)
    {
        if (State == PhilosopherState.Eating) return;

        var view = BuildView();
        var decision = strategy.Decide(in view);

        switch (decision)
        {
            case ActionDecision.None:
                LastActionHint = null;
                break;

            case ActionDecision.TakeLeftFork:
                TryTakeFork(side: ForkSide.Left);
                break;

            case ActionDecision.TakeRightFork:
                TryTakeFork(side: ForkSide.Right);
                break;

            case ActionDecision.ReleaseLeftFork:
                if (HasLeftFork)
                {
                    _leftFork.Release();
                    HasLeftFork = false;
                    LastActionHint = "ReleaseLeftFork";
                }
                break;

            case ActionDecision.ReleaseRightFork:
                if (HasRightFork)
                {
                    _rightFork.Release();
                    HasRightFork = false;
                    LastActionHint = "ReleaseRightFork";
                }
                break;
        }

        // Обе вилки у нас
        if (State == PhilosopherState.Hungry && HasLeftFork && HasRightFork)
        {
            State = PhilosopherState.Eating;
            _eatingStepsLeft = _rng.NextInclusive(Timing.EatMin, Timing.EatMax);
            EatenCount++;
            LastActionHint = "StartEating";
            
            TotalHungrySteps += WaitingInHungry;
            
            _leftFork.BeginEatingUse(Id);
            _rightFork.BeginEatingUse(Id);
        }
    }

    private void TryTakeFork(ForkSide side)
    {
        if (State != PhilosopherState.Hungry) return;

        if (side == ForkSide.Left)
        {
            if (!HasLeftFork && _takingLeftStepsLeft == 0 && _leftFork.TryReserve(Id))
            {
                _takingLeftStepsLeft = Timing.TakeForkDuration;
                LastActionHint = "TakeLeftFork";
            }
        }
        else
        {
            if (!HasRightFork && _takingRightStepsLeft == 0 && _rightFork.TryReserve(Id))
            {
                _takingRightStepsLeft = Timing.TakeForkDuration;
                LastActionHint = "TakeRightFork";
            }
        }
    }
}
