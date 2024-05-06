package View.Panels;

import Model.Resources;

import javax.swing.*;
import java.awt.*;
import java.awt.image.ImageObserver;

public class GameArea {
    private JFrame _frame;
    private JPanel _panel;

    public GameArea() {
        createFrame();
        createPanel();
        _frame.setContentPane(_panel);
        _frame.setLayout(new BorderLayout());
        _frame.setVisible(true);
    }

    private void createFrame() {
        _frame = new JFrame(Resources.MAIN_MENU_NAME);
        _frame.setExtendedState(JFrame.MAXIMIZED_BOTH);
        _frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }

    private void createPanel() {
        _panel = new JPanel() {
            @Override
            protected void paintComponent(Graphics g) {
                super.paintComponent(g);
                drawBackground(g, getHeight(), getWidth(), this);
            }
        };
        //_panel.setOpaque(true);
    }

    private void drawBackground(Graphics g, int height, int width, ImageObserver observer) {
        Image middleImage = new ImageIcon(Resources.PATH_GAME_AREA_MIDDLE).getImage();
        Image backImage = new ImageIcon(Resources.PATH_GAME_AREA_BACK).getImage();
        g.drawImage(backImage, 0, 0, width, height, observer);
        g.drawImage(middleImage, width / 3, 0, width / 3, height, observer);
//        Image sideImage = new ImageIcon(Resources.PATH_GAME_AREA_SIDE).getImage();
//        g.drawImage(sideImage, 0, 0, width / 3, height, observer);
//        g.drawImage(sideImage, width * 2 / 3, 0, width / 3, height, observer);
    }
}

