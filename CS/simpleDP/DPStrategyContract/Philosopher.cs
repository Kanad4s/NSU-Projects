using DPStrategyContract.States;

namespace DPStrategyContract;

public class Philosopher
{
    public string Name { get; }
    public PhilosopherState State { get; set; }

    public int StateDuration { get; private set; } = 0;

    public int MealsEaten = 0;
    public Fork LeftFork { get; set; }
    public Fork RightFork { get; set; }

    private int _thinkingMin { get; }
    private int _thinkingMax { get; }
    private int _eatingMin { get; }
    private int _eatingMax { get; }

    public Philosopher(string name, PhilosopherState state, Fork leftFork, Fork rightFork,
        int thinkingMin, int thinkingMax, int eatingMin, int eatingMax)
    {
        Name = name;
        State = state;
        LeftFork = leftFork;
        RightFork = rightFork;
        _thinkingMin = thinkingMin;
        _thinkingMax = thinkingMax;
        _eatingMin = eatingMin;
        _eatingMax = eatingMax;
    }

    public void StartThinking()
    {
        LeftFork.Release();
        RightFork.Release();
        State = PhilosopherState.Thinking;
        StateDuration = Random.Shared.Next(_thinkingMin, _thinkingMax + 1);
    }

    public bool TryEating()
    {
        if (LeftFork.Owner == this && RightFork.Owner == this)
        {
            State = PhilosopherState.Eating;
            StateDuration = Random.Shared.Next(_eatingMin, _eatingMax + 1);
            return true;
        }

        return false;
    }

    public bool Step()
    {
        if (StateDuration > 1)
        {
            StateDuration--;
            return true;
        }

        if (State == PhilosopherState.Eating)
        {
            MealsEaten++;
            StartThinking();
            return true;
        }
        else if (State == PhilosopherState.Thinking)
        {
            State = PhilosopherState.Hungry;
            StateDuration = 0;
            return true;
        } 

        return false;
    }
}