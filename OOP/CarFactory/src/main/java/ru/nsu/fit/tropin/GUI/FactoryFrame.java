package ru.nsu.fit.tropin.GUI;

import ru.nsu.fit.tropin.factory.product.auto.Accessory;
import ru.nsu.fit.tropin.factory.product.auto.Auto;
import ru.nsu.fit.tropin.factory.product.auto.Body;
import ru.nsu.fit.tropin.factory.product.auto.Motor;
import ru.nsu.fit.tropin.factory.warehouse.Warehouse;

import javax.swing.*;
import java.awt.*;

public class FactoryFrame extends JFrame {

    public final static Color BACKGROUND_COLOR = new Color(147, 255, 108);
    private static final String TITLE_FRAME = "AUTO Factory";

    private final GUIController controller;

    private final DetailInfoPanel accessoryInfoPanel;
    private final DetailInfoPanel  bodyInfoPanel;
    private final DetailInfoPanel motorInfoPanel;
    private final ProductInfoPanel  autoInfoPanel;
    private final DealerInfoPanel dealerInfoPanel;


    public FactoryFrame(GUIController controller) {
        this.controller = controller;

        this.accessoryInfoPanel = new DetailInfoPanel(controller, Warehouse.ACCESSORY_WAREHOUSE_NAME, Accessory.class);
        this.bodyInfoPanel = new DetailInfoPanel(controller, Warehouse.BODY_WAREHOUSE_NAME, Body.class);
        this.motorInfoPanel = new DetailInfoPanel(controller, Warehouse.MOTOR_WAREHOUSE_NAME, Motor.class);
        this.autoInfoPanel = new ProductInfoPanel(controller, Warehouse.AUTO_WAREHOUSE_NAME, Auto.class);
        this.dealerInfoPanel = new DealerInfoPanel(controller);

        init();
        controller.startFactory();

        this.setVisible(true);
        this.setDefaultCloseOperation(EXIT_ON_CLOSE);
    }

    public void init(){
        this.setTitle(TITLE_FRAME);
        this.setSize(500, 500);
        this.setResizable(false);

        this.setLayout(new GridLayout(2, 3, 5, 5));

        this.getContentPane().add(accessoryInfoPanel);
        this.getContentPane().add(motorInfoPanel);
        this.getContentPane().add(bodyInfoPanel);
        this.getContentPane().add(autoInfoPanel);
        this.getContentPane().add(dealerInfoPanel);
    }
}
