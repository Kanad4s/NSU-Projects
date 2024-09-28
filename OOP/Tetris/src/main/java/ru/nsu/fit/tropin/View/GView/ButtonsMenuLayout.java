package ru.nsu.fit.tropin.View.GView;

import java.awt.*;

import static ru.nsu.fit.tropin.Model.Resources.BUTTON_HEIGHT;
import static ru.nsu.fit.tropin.Model.Resources.BUTTON_WIDTH;

public class ButtonsMenuLayout implements LayoutManager {

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
            comp.setBounds(parent.getWidth() / 2 - BUTTON_WIDTH / 2,
                    parent.getHeight() / 2 - BUTTON_HEIGHT * parent.getComponentCount() / 2 + i * BUTTON_HEIGHT,
                    BUTTON_WIDTH, BUTTON_HEIGHT);
        }
    }
}
