package View.GView;

import Model.Resources;
import View.DrawBlock;

import java.awt.*;

public class ShapesView {
    static public  void drawBackGround(Graphics graphics, Color[][] placedShape, int gameAreaWidth, int gameAreaHeight) {
        int blockSize = gameAreaWidth / Resources.BLOCKS_IN_ROW;
        for (int y = 0; y < gameAreaHeight / blockSize; y++) {
            for (int x = 0; x < Resources.BLOCKS_IN_ROW; x++) {
                Color color = placedShape[y][x];
                if (color != null)
                    DrawBlock.draw(graphics, x, y, color,
                            gameAreaWidth , gameAreaWidth, gameAreaHeight);
            }
        }
    }
}
