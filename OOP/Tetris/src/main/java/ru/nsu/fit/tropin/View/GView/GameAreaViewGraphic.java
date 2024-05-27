package ru.nsu.fit.tropin.View.GView;

import ru.nsu.fit.tropin.Controller.LaunchController;
import ru.nsu.fit.tropin.Model.GameArea;
import ru.nsu.fit.tropin.Model.Resources;
import ru.nsu.fit.tropin.View.GameAreaView;

import javax.swing.*;
import java.awt.*;
import java.awt.image.ImageObserver;

public class GameAreaViewGraphic implements GameAreaView {
    private JFrame _frame;
    private JPanel _panel;
    private JButton _buttonRestart, _buttonMenu;
    private GameArea _gameArea;

    public GameAreaViewGraphic() {};

    @Override
    public void setGameArea(GameArea gameArea) {
        _gameArea = gameArea;
    }

    @Override
    public void showArea() {
        createFrame();
        createPanel();
        _frame.setContentPane(_panel);
        addButtons();
        _frame.setVisible(true);
    }


    @Override
    public void addPlayerToRecord(String name, int points) {
        RecordTable recordTable = new RecordTable();
        recordTable.addPlayer(name, points);
        recordTable.fillRecordTable();
    }

    @Override
    public int getAreaHeight() {
        return _frame.getHeight();
    }

    @Override
    public int getAreaWidth() {
        return _frame.getWidth();
    }

    @Override
    public JFrame getFrame() {
        return _frame;
    }

    @Override
    public void update() {
        _panel.repaint();
    }

    @Override
    public String getPlayerName() {
        return JOptionPane.showInputDialog("Game Over\n Please, input your name.");
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
                _gameArea.setAreaSize(getAreaHeight(), getAreaWidth());
                drawBackground(g, getHeight(), getWidth(), this);
                drawPlayerPoints(g);
                DrawShapeGraphic.drawShapeArea(g, _gameArea.getCurrentShape(), _gameArea.getAreaWidth(), _gameArea.getAreaHeight());
                ShapesView.drawBackGround(g, _gameArea.getPlacedShapes(), _gameArea.getAreaWidth(), _gameArea.getAreaHeight());
                drawNextShape(g, _frame.getWidth(), this);
            }
        };
        _panel.setOpaque(true);
    }

    private void drawBackground(Graphics g, int height, int width, ImageObserver observer) {
        Image middleImage = new ImageIcon(Resources.PATH_GAME_AREA_MIDDLE).getImage();
        Image backImage = new ImageIcon(Resources.PATH_GAME_AREA_BACK).getImage();
        g.drawImage(backImage, 0, 0, width, height, observer);
        g.drawImage(middleImage, width / 3, 0, _gameArea.getAreaWidth(), _gameArea.getAreaHeight(), observer);
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
            _gameArea.restart();
        });
    }

    private void createButtonMenu() {
        _buttonMenu = new JButton("Menu");
        _buttonMenu.addActionListener(e -> {
            _frame.dispose();
            LaunchController launchController = new LaunchController();
            launchController.setMenu(new MainMenuGraphic());
            launchController.launchMainMenu();
        });
    }

    private void drawPlayerPoints(Graphics g) {
        Font font = new Font("Points", Font.BOLD, Resources.TEXT_POINTS_SIZE);
        g.setFont(font);
        g.setColor(new Color(88, 114, 140));
        g.drawString("POINTS", _frame.getWidth() * 2 / 3, Resources.TEXT_POINTS_SIZE);
        g.drawString(_gameArea.getPoints().toString(), _frame.getWidth() * 2 / 3, 2 * Resources.TEXT_POINTS_SIZE);
    }

    private void drawNextShape(Graphics g, int areaWidth, ImageObserver observer) {
        int blockSize = _gameArea.getAreaWidth() / Resources.BLOCKS_IN_ROW;
        blockSize = blockSize - blockSize % 10;
        Font font = new Font("Next shape", Font.BOLD, Resources.TEXT_POINTS_SIZE);
        g.setFont(font);
        g.setColor(new Color(88, 114, 140));
        int widthShift = blockSize * 4 + blockSize / 2;
        g.drawString("SHAPE", areaWidth / 3 - widthShift, Resources.TEXT_POINTS_SIZE);
        Image backImage = new ImageIcon(Resources.PATH_GAME_AREA_MIDDLE).getImage();
        g.drawImage(backImage, areaWidth / 3 - widthShift, Resources.TEXT_POINTS_SIZE + 4,
                blockSize * 4, blockSize * 2, observer);
        DrawShapeGraphic.drawShape(g, _gameArea.getNextShape(), areaWidth / 3 - widthShift,
                Resources.TEXT_POINTS_SIZE + 4, blockSize);
    }
}

