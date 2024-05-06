package Model;

import Model.MyObserver;

public interface MyObservable {
    void registerObserver(MyObserver o);
    void removeObserver(MyObserver o);
    void notifyObservers();

}
