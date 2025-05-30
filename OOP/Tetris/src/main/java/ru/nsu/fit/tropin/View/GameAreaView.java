package ru.nsu.fit.tropin.View;

import ru.nsu.fit.tropin.Model.GameArea;
import ru.nsu.fit.tropin.Model.MyObserver;

import javax.swing.*;

public interface GameAreaView extends MyObserver {
    public void setGameArea(GameArea gameArea);
    public void showArea();
    public void addPlayerToRecord(String name, int points);
    public int getAreaHeight();
    public int getAreaWidth();
    public JFrame getFrame();
    public String getPlayerName();
}
