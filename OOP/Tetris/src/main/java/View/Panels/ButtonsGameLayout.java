package View.Panels;

import java.awt.*;

import static Model.Resources.BUTTON_HEIGHT;
import static Model.Resources.BUTTON_WIDTH;

public class ButtonsGameLayout implements LayoutManager {

    @Override
    public void addLayoutComponent(String name, Component comp) {

    }

    @Override
    public void removeLayoutComponent(Component comp) {

    }

    @Override
    public Dimension preferredLayoutSize(Container parent) {
        return null;
    }

    @Override
    public Dimension minimumLayoutSize(Container parent) {
        return null;
    }

    @Override
    public void layoutContainer(Container parent) {
        for (int i = 0; i < parent.getComponentCount(); i++) {
            Component comp = parent.getComponent(i);
            comp.setBounds(parent.getWidth() / 3 - BUTTON_WIDTH + i * (parent.getWidth() / 3 + BUTTON_WIDTH),
                    parent.getHeight() - BUTTON_HEIGHT,
                    BUTTON_WIDTH, BUTTON_HEIGHT);
        }
    }
}
