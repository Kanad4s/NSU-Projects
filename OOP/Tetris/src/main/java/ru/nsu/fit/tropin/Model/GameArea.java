package ru.nsu.fit.tropin.Model;

import ru.nsu.fit.tropin.Model.Shape.*;

import java.awt.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;

public class GameArea implements MyObservable {
    private ArrayList<MyObserver> _observers;
    private MyShape[] _shapes;
    private Random _random;
    private MyShape _currentShape;
    private Integer _points;
    private Color[][] _placedShapes;
    private int _areaWidth;
    private int _areaHeight;
    private  int _areaBlockSize;

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

    public void generateShape() {
        _currentShape = _shapes[_random.nextInt(_shapes.length)];
        _currentShape.spawn();
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
        addPoints();
        moveShapeToBackground();
        clearLines(_areaHeight, _areaWidth);
        generateShape();
    }

    public boolean isBlockOutOfBounds() {
        boolean retValue = false;
//        for (int i = 0; i < Resources.BLOCKS_IN_ROW - 1; i++) {
//            System.out.print((boolean)(_placedShapes[_areaHeight / _areaBlockSize - 1][i] != null) + " ");
//            if (_placedShapes[_areaHeight / _areaBlockSize - 1][i] != null) {
//                retValue = true;
//            }
//        }
//        System.out.println();
        return (_currentShape.getY() <= 0 && _points > 20);
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
        _currentShape.spawn();
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
                clearLine(row, Resources.BLOCKS_IN_ROW);
                shiftRowsDown(row, Resources.BLOCKS_IN_ROW);
                clearLine(0, Resources.BLOCKS_IN_ROW);
            }
        }
        notifyObservers();
    }

    private void clearLine(int row, int blocksInRow) {
        for (int i = 0; i < blocksInRow; i++) {
            _placedShapes[row][i] = null;
        }
    }

    private void shiftRowsDown(int curRow, int blocksInRow) {
        for (int row = curRow; row > 0; row--) {
            if (blocksInRow >= 0) System.arraycopy(_placedShapes[row - 1], 0, _placedShapes[row], 0, blocksInRow);
        }
    }
}
