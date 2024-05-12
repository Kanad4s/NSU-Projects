package Model;

import Model.Shape.MyShape;

import java.awt.*;

public class CheckMovement {
    static public boolean isBarrier(MyShape shape, Color[][] placedShape, int areaWidth, int areaHeight) {
        int width = shape.getWidth();
        int height = shape.getHeight();
        if (shape.getY() + height >= areaHeight / Resources.BLOCKS_IN_ROW - 1) {
            return true;
        }

        for (int column = 0; column < width; column++) {
            for (int row = height - 1; row >= 0; row--) {
                int x = column + shape.getX();
                int y = row + shape.getY();
                if (shape.isShape(column, row) && placedShape[y + 1][x] != null)
                    return true;
            }
        }
        return false;
    }
}
