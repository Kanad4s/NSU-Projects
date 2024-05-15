package Model.Shape;

import Model.Resources;

import java.awt.*;
import java.util.Random;

public abstract class MyShape {
    private int _x;
    private int _y;
    private int _rotationPosition;
    private boolean[][] _shape;
    private boolean[][][] _shapeRotattions;
    private Color _currentColor;
    private Random _random = new Random();
    private Color[] _colors = new Color[] {
            Color.red, Color.green, Color.blue, Color.yellow, Color.orange, Color.pink,
            Color.cyan, Color.magenta, Color.gray, Color.darkGray, Color.white, Color.LIGHT_GRAY
    };
    private final int CNT_ROTATIONS = 4;

    public MyShape(boolean[][] shape) {
        _shape = shape;
        generateShapeRotations();
        setColor();
    }

    public void spawn() {
        _x = Resources.BLOCKS_IN_ROW / 2 - _random.nextInt(3);
        _y = 0;
        setColor();
    }

    private void setColor() {
        Color newColor = _colors[_random.nextInt(_colors.length)];
        while (_currentColor == newColor) {
            newColor = _colors[_random.nextInt(_colors.length)];
        }
        _currentColor = newColor;
    }

    private void generateShapeRotations() {
        _shapeRotattions = new boolean[CNT_ROTATIONS][][];
        for (int i = 0; i < CNT_ROTATIONS; ++i) {
            int h = getHeight();
            int w = getWidth();
            _shapeRotattions[i] = new boolean[h][w];
            for (int y = 0; y < h; ++y) {
                for (int x = 0; x < w; ++x) {
                    _shapeRotattions[i][y][x] = _shape[w - x - 1][y];
                }
            }
            _shape = _shapeRotattions[i];
        }
    };

    public int getWidth() {
        return _shape.length;
    }

    public int getHeight() {
        return _shape[0].length;
    }

    public boolean isShape(int x, int y) {
        return _shape[x][y];
    }

    public Color getColor() {
        return _currentColor;
    }

    public int getY() {
        return _y;
    }

    public void setY(int y) {
        _y = y;
    }

    public int getX() {
        return _x;
    }

    public void setX(int x) {
        _x = x;
    }

    public void moveDown() {
        _y += 1;
    }
    public void moveUp() {
        _y -= 1;
    }
    public void moveLeft() {
        _x -= 1;
    }
    public void moveRight() {
        _x += 1;
    }

    public void moveRotate() {
        nextRotation();
    }

    public int getLeftSide() {
        return _x;
    }

    public int getRightSide() {
        return _x + getWidth();
    }

    public void nextRotation() {
        _shape = _shapeRotattions[(++_rotationPosition) % CNT_ROTATIONS];
    }

    public void previousRotation() {
        _shape = _shapeRotattions[(--_rotationPosition) % CNT_ROTATIONS];
    }
}
