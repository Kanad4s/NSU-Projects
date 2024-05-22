package ru.nsu.fit.tropin.View.TextView;

import ru.nsu.fit.tropin.Model.Shape.MyShape;

import java.awt.*;

public class DrawAreaViewText {
    public static void drawArea(MyShape shape, int areaHeight, int areaWidth, Color[][] placedShapes) {
        for (int i = 0; i < areaHeight; i++) {
            DrawBlockText.drawBound();
            for (int j = 0; j < areaWidth; j++) {
                if (isShape(shape, j , i) || placedShapes[i][j] != null) {
//                if (shape.isShape(j, i) || placedShapes[i][j] != null) {
                    DrawBlockText.drawBlock();
                } else {
                    DrawBlockText.drawSpace();
                }
            }
            DrawBlockText.drawBound();
            System.out.println();
        }
        for (int i = 0; i < areaWidth + 2; i++) {
            DrawBlockText.drawBound();
        }
        System.out.println();
    }

    private static boolean isShape(MyShape shape, int x, int y) {
        if ((shape.getX() <= x && x < shape.getX() + shape.getWidth()) &&
        (shape.getY() <= y && y < shape.getY() + shape.getHeight())) {
            return shape.isShape(shape.getX() + shape.getWidth() - x - 1, shape.getY() + shape.getHeight() - y - 1);
        }
        return false;
    }
}
