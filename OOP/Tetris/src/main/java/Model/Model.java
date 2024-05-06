package Model;

import java.util.ArrayList;

public class Model implements MyObservable {
    private ArrayList<MyObserver> _observers;

    @Override
    public void registerObserver(MyObserver o) {
        _observers.add(o);
    }

    @Override
    public void removeObserver(MyObserver o) {
        _observers.remove(o);
    }

    @Override
    public void notifyObservers() {
        for (MyObserver o : _observers) {
            o.update();
        }
    }
}
