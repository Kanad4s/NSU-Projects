package ru.nsu.fit.tropin.View.TextView;

import ru.nsu.fit.tropin.Model.GameArea;
import ru.nsu.fit.tropin.Model.Resources;
import ru.nsu.fit.tropin.View.GameAreaView;

import javax.swing.*;

public class GameAreaViewText implements GameAreaView {


    @Override
    public void setGameArea(GameArea gameArea) {

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
        return null;
    }

    @Override
    public void update() {

    }
}
