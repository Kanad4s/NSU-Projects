package View.Panels;

import Controller.launchController;
import Model.Model;
import Model.MyObserver;
import Model.Resources;
import View.DrawShape;

import javax.swing.*;
import java.awt.*;
import java.awt.image.ImageObserver;

public class GameAreaView implements MyObserver {
    private JFrame _frame;
    private JPanel _panel;
    private JButton _buttonRestart, _buttonMenu;
    private Model _model;

    public GameAreaView(Model model) {
        _model = model;
        createFrame();
        createPanel();
        _frame.setContentPane(_panel);
        addButtons();
    }

    public void showArea() {
        _frame.setVisible(true);
    }

    public JFrame getFrame() {
        return _frame;
    }

    @Override
    public void update() {
        _panel.repaint();
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
                _model.setAreaSize(getHeight(), getWidth());
                drawBackground(g, getHeight(), getWidth(), this);
                drawPlayerPoints(g);
                DrawShape.drawShape(g, _model.getCurrentShape(), _model.getAreaWidth());
            }
        };
        _panel.setOpaque(true);
    }

    private void drawBackground(Graphics g, int height, int width, ImageObserver observer) {
        Image middleImage = new ImageIcon(Resources.PATH_GAME_AREA_MIDDLE).getImage();
        Image backImage = new ImageIcon(Resources.PATH_GAME_AREA_BACK).getImage();
        g.drawImage(backImage, 0, 0, width, height, observer);
        g.drawImage(middleImage, width / 3, 0, _model.getAreaWidth(), _model.getAreaHeight(), observer);
    }

    private void addButtons() {
        createButtons();
        _frame.getContentPane().add(_buttonMenu);
        _frame.getContentPane().add(_buttonRestart);
        _frame.setLayout(new ButtonsGameLayout());
    }

    private void createButtons() {
        createButtonMenu();
        createButtonRestart();
    }

    private void createButtonRestart() {
        _buttonRestart = new JButton("Restart");
        _buttonRestart.addActionListener(e -> {
            //todo
        });
    }

    private void createButtonMenu() {
        _buttonMenu = new JButton("Menu");
        _buttonMenu.addActionListener(e -> {
            _frame.dispose();
            launchController gameController = new launchController();
            gameController.launchMainMenu();
        });
    }

    private void drawPlayerPoints(Graphics g) {
        Font font = new Font("Points", Font.BOLD, Resources.TEXT_POINTS_SIZE);
        g.setFont(font);
        g.setColor(new Color(88, 114, 140));
        g.drawString("POINTS", _frame.getWidth() * 2 / 3, Resources.TEXT_POINTS_SIZE);
        g.drawString(_model.getPoints().toString(), _frame.getWidth() * 2 / 3, 2 * Resources.TEXT_POINTS_SIZE);
    }
}

