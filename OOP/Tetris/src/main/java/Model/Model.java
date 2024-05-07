package Model;

import Model.Shape.*;

import java.awt.*;
import java.util.ArrayList;
import java.util.Random;

public class Model implements MyObservable {
    private ArrayList<MyObserver> _observers;
    private MyShape[] _shapes;
    private Random _random;
    private MyShape _currentShape;

    public Model() {
        _random = new Random();
        _observers = new ArrayList<MyObserver>();
        _shapes = new MyShape[] {new Hero(), new Teewee(), new Smashboy(), new Rhode(), new Ricky()};
    }

    public void spawnShape() {
        MyShape _currentShape = _shapes[_random.nextInt(_shapes.length)];
        _currentShape.spawn();
    }

    public void moveShapeUp() {
        _currentShape.moveUp();
        notifyObservers();
    }

    public void moveShapeDown() {
        _currentShape.moveDown();
        notifyObservers();
    }

    public void moveShapeLeft() {
        _currentShape.moveLeft();
        notifyObservers();
    }

    public void moveShapeRight() {
        _currentShape.moveRight();
        notifyObservers();
    }

    public void moveShapeRotate() {
        _currentShape.moveRotate();
        notifyObservers();
    }

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
