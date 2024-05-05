package View.Panels;

import Model.Resources;

import java.awt.*;

public class LayoutMy implements LayoutManager {

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
            comp.setBounds(parent.getWidth() / 2 - Resources.BUTTON_WIDTH / 2,
                    parent.getHeight() / 2 - Resources.BUTTON_HEIGHT * parent.getComponentCount() / 2 + i * Resources.BUTTON_HEIGHT,
                    Resources.BUTTON_WIDTH, Resources.BUTTON_HEIGHT);
        }
    }
}
