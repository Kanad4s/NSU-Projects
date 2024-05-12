package View;

import Model.Resources;

import java.awt.*;

public class DrawBlock {
    public static void draw(Graphics g, int x, int y, Color color, int offsetWidth, int gameAreaWidth, int gameAreaHeight) {
        int blockSize = gameAreaWidth / Resources.BLOCKS_IN_ROW;
        int heightShift = gameAreaHeight % blockSize;
        g.setColor(color);
        g.fillRect(x * blockSize + offsetWidth, y * blockSize + heightShift, blockSize, blockSize);
        g.setColor(Color.black);
        g.drawRect(x * blockSize + offsetWidth, y * blockSize + heightShift, blockSize, blockSize);
    }
}
