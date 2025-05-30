package ru.nsu.fit.tropin.Model;

import ru.nsu.fit.tropin.Model.Shape.MyShape;

import java.awt.*;

public class CheckMovement {
    static public boolean isBarrier(MyShape shape, Color[][] placedShape, int areaWidth, int areaHeight) {
        int width = shape.getWidth();
        int height = shape.getHeight();
        if (shape.getY() + height >= areaHeight / (areaWidth / Resources.BLOCKS_IN_ROW)) {
            return true;
        }
        for (int column = 0; column < width ; column++) {
            for (int row = height - 1; row >= 0; row--) {
                int x = column + shape.getX();
                int y = row + shape.getY();
                if (shape.isShape(column, row) && placedShape[y + 1][x] != null)
                    return true;
            }
        }
        return false;
    }

    static public boolean checkMoveUp(MyShape shape){
        return !(shape.getY() <= 0);
    }

    static public boolean checkMoveLeftShape(MyShape shape, Color[][] placedShape) {
        if (shape.getLeftSide() == 0)
            return false;

        int w = shape.getWidth();
        int h = shape.getHeight();
        for (int row = 0; row < h; ++row) {
            for (int column = 0; column < w; ++column) {
                if (shape.isShape(column, row) &&
                        placedShape[row + shape.getY()][column + shape.getX() - 1] != null)
                    return false;
            }
        }
        return true;
    }

    static public boolean checkMoveRightShape(MyShape shape, Color[][] placedShape, int areaWidth){
        if (shape.getRightSide() >= Resources.BLOCKS_IN_ROW)
            return false;

        int w = shape.getWidth();
        int h = shape.getHeight();
        for (int row = 0; row < h; ++row) {
            for (int column = 0; column < w; ++column) {
                if (shape.isShape(column, row) &&
                        placedShape[row + shape.getY()][column + shape.getX() + 1] != null) {
                    return false;
                }
            }
        }
        return true;
    }

    static public boolean checkMoveRotateShape(MyShape shape, Color[][] placedShape, int areaWidth, int areaHeight){
        shape.nextRotation();
        if (!checkMoveRightShape(shape, placedShape, areaWidth) ||
                !checkMoveLeftShape(shape, placedShape) ||
                isBarrier(shape, placedShape, areaWidth, areaHeight))
            {
            shape.previousRotation();
            return false;
        }
        shape.previousRotation();
        return true;
    }
}
