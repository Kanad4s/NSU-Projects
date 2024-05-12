package View;

import Model.Shape.MyShape;
import java.awt.*;

public class DrawShape {
    public static void drawShape(Graphics g, MyShape shape, int gameAreaWidth) {
        int height = shape.getHeight();
        int width = shape.getWidth();
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (shape.isShape(x, y)) {
                    DrawBlock.draw(g, x + shape.getX(), y + shape.getY(), shape.getColor(),
                            gameAreaWidth , gameAreaWidth);
                }
            }
        }
    }

    public static void drawShapesBackground(Graphics g, MyShape shape) {

    }
}
