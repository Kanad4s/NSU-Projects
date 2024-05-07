package Model.Shape;

import Model.Resources;

public class MyShape {
    private int _x;
    private int _y;
    private boolean[][] _shape;
    private int _colorsCount;
    private int _currentColorIndex;

    public MyShape(boolean[][] shape) {
        _shape = shape;
        _colorsCount = Resources.SHAPE_COLORS_COUNT;
        _currentColorIndex = 0;
    }

    public int get_y() {
        return _y;
    }

    public void set_y(int y) {
        _y = y;
    }

    public int get_x() {
        return _x;
    }

    public void set_x(int x) {
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
        //todo
    }

    public void spawn() {
        _x = 0;
        _y = 0;
    }

    public void nextColor() {
        _currentColorIndex = (_currentColorIndex + 1) % _colorsCount;
    }
}
