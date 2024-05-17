package ru.nsu.fit.tropin.View.GView;

import ru.nsu.fit.tropin.Model.Shape.MyShape;

import java.awt.*;

public class DrawShapeGraphic {
    public static void drawShape(Graphics g, MyShape shape, int gameAreaWidth, int gameAreaHeight) {
        int shapeHeight = shape.getHeight();
        int shapeWidth = shape.getWidth();
        for (int y = 0; y < shapeHeight; y++) {
            for (int x = 0; x < shapeWidth; x++) {
                if (shape.isShape(x, y)) {
                    DrawBlockGraphic.draw(g, x + shape.getX(), y + shape.getY(), shape.getColor(),
                            gameAreaWidth , gameAreaWidth, gameAreaHeight);
                }
            }
        }
    }
}
