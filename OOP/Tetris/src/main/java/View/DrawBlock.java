package View;

import java.awt.*;

public class DrawBlock {
    public static void draw(Graphics g, int x, int y, Color color, int offsetWidth, int offsetShared) {
        g.setColor(color);
        g.fillRect(x * offsetShared + offsetWidth, y * offsetShared, offsetShared, offsetShared);
        g.setColor(Color.black);
        g.drawRect(x * offsetShared + offsetWidth, y * offsetShared, offsetShared, offsetShared);
    }
}
