package ru.nsu.fit.tropin.Model;

public interface MyObservable {
    void registerObserver(MyObserver o);
    void removeObserver(MyObserver o);
    void notifyObservers();

}
