package ru.nsu.fit.tropin.GUI;

import lombok.Getter;
import ru.nsu.fit.tropin.factory.product.Product;
import ru.nsu.fit.tropin.factory.product.auto.Accessory;
import ru.nsu.fit.tropin.factory.product.auto.Body;
import ru.nsu.fit.tropin.factory.product.auto.Motor;
import ru.nsu.fit.tropin.observer.Observer;

import javax.swing.*;
import java.awt.*;

import static ru.nsu.ccfit.chernovskaya.GUI.FactoryFrame.BACKGROUND_COLOR;

public class ProductInfoPanel extends JPanel implements Observer {

    private final String title;
    private final Class<?> _class;
    private final GUIController controller;

    private final JLabel nameLabel = new JLabel();
    private final JLabel totalProductLabel = new JLabel();
    private final JLabel currentProductLabel = new JLabel();
    @Getter private final GridBagConstraints gridBagConstraints = new GridBagConstraints();

    protected ProductInfoPanel(GUIController controller, String title, Class<? extends Product> _class) {

        this.controller = controller;
        this.title = title;
        this._class = _class;

        nameLabel.setText(title);
        totalProductLabel.setText("Total product: 0");
        currentProductLabel.setText("Current product: 0");

        if (_class == Accessory.class)
            controller.addAccessoryWarehouseObserver(this);
        else if (_class == Body.class)
            controller.addBodyWarehouseObserver(this);
        else if (_class == Motor.class)
            controller.addMotorWarehouseObserver(this);
        else
            controller.addAutoWarehouseObserver(this);

        setLayout(new GridBagLayout());

        gridBagConstraints.fill = GridBagConstraints.HORIZONTAL;
        gridBagConstraints.gridy = 3;
        add(currentProductLabel, gridBagConstraints);

        gridBagConstraints.fill = GridBagConstraints.HORIZONTAL;
        gridBagConstraints.gridy = 2;
        add(totalProductLabel, gridBagConstraints);

        gridBagConstraints.fill = GridBagConstraints.HORIZONTAL;
        gridBagConstraints.gridy = 1;
        add(nameLabel, gridBagConstraints);

        this.setBackground(BACKGROUND_COLOR);

    }

    @Override
    public void update(int currentProductCount, int totalProductCount) {
        totalProductLabel.setText("Total product: " + totalProductCount);
        currentProductLabel.setText("Current product: " + currentProductCount);
    }
}