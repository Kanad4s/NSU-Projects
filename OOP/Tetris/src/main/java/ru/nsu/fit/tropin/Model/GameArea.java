package ru.nsu.fit.tropin.Model;

import ru.nsu.fit.tropin.Model.Shape.*;

import java.awt.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;

public class GameArea implements MyObservable {
    private ArrayList<MyObserver> _observers;
    private final MyShape[] _shapes;
    private final Random _random;
    private MyShape _currentShape;
    private MyShape _nextShape;
    private Integer _points;
    private Color[][] _placedShapes;
    private int _areaWidth;
    private int _areaHeight;
    private int _areaBlockSize;

    public GameArea() {
        _points = 0;
        _random = new Random();
        _observers = new ArrayList<MyObserver>();
        _shapes = new MyShape[] {new Hero(), new Teewee(), new Smashboy(), new Rhode(), new Ricky()};
        generateShape();
    }

    public Integer getPoints() {
        return _points;
    }

    public Color[][] getPlacedShapes() {
        return _placedShapes;
    }

    public MyShape getCurrentShape() {
        return _currentShape;
    }

    public MyShape getNextShape() {
        return _nextShape;
    }

    public void generateShape() {
        System.out.println("Generating shape...");
        _currentShape = makeNextShape();
        _nextShape = makeNextShape();
    }

    public void moveShapeUp() {
        if (CheckMovement.checkMoveUp(_currentShape)) {
            _currentShape.moveUp();
            notifyObservers();
        }
    }

    public void moveShapeDown() {
        if (!CheckMovement.isBarrier(_currentShape, _placedShapes, _areaWidth, _areaHeight)) {
            _currentShape.moveDown();
        }
        notifyObservers();
    }

    public void moveShapeLeft() {
        if (CheckMovement.checkMoveLeftShape(_currentShape, _placedShapes)) {
            _currentShape.moveLeft();
            notifyObservers();
        }
    }

    public void moveShapeRight() {
        if (CheckMovement.checkMoveRightShape(_currentShape, _placedShapes, _areaWidth)) {
            _currentShape.moveRight();
            notifyObservers();
        }
    }

    public void moveShapeRotate() {
        if (CheckMovement.checkMoveRotateShape(_currentShape, _placedShapes, _areaWidth, _areaHeight)) {
            _currentShape.moveRotate();
            notifyObservers();
        }
    }

    public boolean isShapeMoving() {
        return !CheckMovement.isBarrier(_currentShape, _placedShapes, _areaWidth, _areaHeight);
    }

    public void spawnNextShape() {
        System.out.println("Spawning next shape..." + ableToRespawn());
        if (!ableToRespawn()) {
            restart();
            return;
        }
        addPoints();
        moveShapeToBackground();
        clearLines(_areaHeight, _areaWidth);
        _currentShape = _nextShape;
        _nextShape = makeNextShape();
    }

    public boolean isBlockOutOfBounds() {
        boolean retValue = false;
        for (int i = 0; i < Resources.BLOCKS_IN_ROW; i++) {
            if (_placedShapes[_areaHeight / _areaBlockSize - 1][i] != null) {
                retValue = true;
            }
        }
        System.out.println("isBlockOutOfBounds: " + (_currentShape.getY() <= 0 && retValue));
        return (_currentShape.getY() <= 0 && retValue);
    }

    public void setAreaSize(int frameHeight, int frameWidth) {
        if (frameWidth != Resources.BLOCKS_IN_ROW) {
            _areaWidth = frameWidth / 3;
            _areaWidth = _areaWidth - _areaWidth % 10;
        } else {
            _areaWidth = frameWidth;
        }
        _areaHeight = frameHeight;
        _areaBlockSize = _areaWidth / Resources.BLOCKS_IN_ROW;
//        System.out.println(_areaHeight + " " + _areaBlockSize);
    }

    public void createPlacedShapes() {
        _placedShapes = new Color[_areaHeight / _areaBlockSize][_areaWidth / _areaBlockSize];
    }

    public int getAreaWidth() {
        return _areaWidth;
    }

    public int getAreaHeight() {
        return _areaHeight;
    }

    public void restart() {
        for (Color[] placedShape : _placedShapes) {
            Arrays.fill(placedShape, null);
        }
        _points = 0;
        _currentShape = makeNextShape();
        _nextShape = makeNextShape();
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

    private MyShape makeNextShape() {
        MyShape nextShape = _shapes[_random.nextInt(_shapes.length)];
        while (nextShape == _currentShape) {
            nextShape = _shapes[_random.nextInt(_shapes.length)];
        }
        nextShape.spawn();
        return nextShape;
    }

    private void addPoints() {
        _points += Resources.POINTS_PER_SHAPE;
    }

    private void moveShapeToBackground() {
        int height = _currentShape.getHeight();
        int width = _currentShape.getWidth();
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (_currentShape.isShape(x, y))
                    _placedShapes[y + _currentShape.getY()][x + _currentShape.getX()] = _currentShape.getColor();
            }
        }
    }

    private void clearLines(int areaHeight, int areaWidth) {
        int cntBlocksInRow;
        for (int row = 0; row < areaHeight / _areaBlockSize; row++) {
            cntBlocksInRow = 0;
            for (int column = 0; column < areaWidth / _areaBlockSize; column++) {
                if (_placedShapes[row][column] != null) ++cntBlocksInRow;
            }
            if (cntBlocksInRow == Resources.BLOCKS_IN_ROW) {
                _points += 100;
                clearLine(row);
                shiftRowsDown(row);
                clearLine(0);
            }
        }
        notifyObservers();
    }

    private void clearLine(int row) {
        for (int i = 0; i < Resources.BLOCKS_IN_ROW; i++) {
            _placedShapes[row][i] = null;
        }
    }

    private void shiftRowsDown(int curRow) {
        for (int row = curRow; row > 0; row--) {
            if (Resources.BLOCKS_IN_ROW >= 0) System.arraycopy(_placedShapes[row - 1], 0, _placedShapes[row],
                    0, Resources.BLOCKS_IN_ROW);
        }
    }

    private boolean ableToRespawn() {
        return _currentShape.getY() >= 1;
    }
}
