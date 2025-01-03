package ru.nsu.fit.tropin.View.TextView;

import ru.nsu.fit.tropin.Model.GameArea;
import ru.nsu.fit.tropin.Model.Resources;
import ru.nsu.fit.tropin.View.GameAreaView;

import javax.swing.*;
import java.awt.*;
import java.util.Scanner;

public class GameAreaViewText implements GameAreaView {
    private JFrame _frame;
    private GameArea _gameArea;

    @Override
    public void setGameArea(GameArea gameArea) {
        _gameArea = gameArea;
    }

    @Override
    public void showArea() {

    }

    @Override
    public void addPlayerToRecord(String name, int points) {
        RecordTableText recordTableText = new RecordTableText();
        recordTableText.addPlayer(name, points);
        recordTableText.fillRecordTable();
        MainMenuText mainMenuText = new MainMenuText();
        mainMenuText.showMainMenu();
    }

    @Override
    public int getAreaHeight() {
        return (int) (Resources.BLOCKS_IN_ROW * 1.5);
    }

    @Override
    public int getAreaWidth() {
        return Resources.BLOCKS_IN_ROW;
    }

    @Override
    public JFrame getFrame() {
        _frame = new JFrame();
        _frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        JPanel panel = new JPanel();
        _frame.setContentPane(panel);
        _frame.setUndecorated(true);
        _frame.setBackground(new Color(0, 0, 0, 0));
        _frame.setVisible(true);
        return _frame;
    }

    @Override
    public String getPlayerName() {
        System.out.println();
        Scanner in = new Scanner(System.in);
        return in.nextLine();
    }

    @Override
    public void update() {
        repaint();
    }

    private void repaint() {
        System.out.flush();
        DrawAreaViewText.printSpace();
        DrawAreaViewText.drawArea(_gameArea.getCurrentShape(), _gameArea.getAreaHeight(), _gameArea.getAreaWidth(),
                _gameArea.getPlacedShapes());
        DrawAreaViewText.drawPoints(_gameArea.getPoints());
        DrawAreaViewText.drawNextShape(_gameArea.getNextShape());
    }
}
