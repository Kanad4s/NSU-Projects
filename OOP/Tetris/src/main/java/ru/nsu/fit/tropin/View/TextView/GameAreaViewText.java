package ru.nsu.fit.tropin.View.TextView;

import ru.nsu.fit.tropin.Model.GameArea;
import ru.nsu.fit.tropin.Model.Resources;
import ru.nsu.fit.tropin.View.GameAreaView;

import javax.swing.*;

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
    public int getAreaHeight() {
        return Resources.BLOCKS_IN_ROW * 2;
    }

    @Override
    public int getAreaWidth() {
        return Resources.BLOCKS_IN_ROW;
    }

    @Override
    public JFrame getFrame() {
        _frame = new JFrame();
        return _frame;
    }

    @Override
    public void update() {
        repaint();
    }

    private void repaint() {
        System.out.flush();
        DrawAreaViewText.drawArea(_gameArea.getCurrentShape(), _gameArea.getAreaHeight(), _gameArea.getAreaWidth(),
                _gameArea.getPlacedShapes());
    }
}
