package View;

import Model.Resources;

import java.awt.*;

public class DrawBlock {
    public static void draw(Graphics g, int x, int y, Color color, int offsetWidth, int shapesAreaWidth) {
        int blockSize = shapesAreaWidth / Resources.BLOCKS_IN_ROW;
        g.setColor(color);
        g.fillRect(x * blockSize + offsetWidth, y * blockSize, blockSize, blockSize);
        g.setColor(Color.black);
        g.drawRect(x * blockSize + offsetWidth, y * blockSize, blockSize, blockSize);
    }
}
