package View;

import Model.GameArea;
import Model.MyObserver;

import javax.swing.*;

public interface GameAreaView extends MyObserver {
    public void setGameArea(GameArea gameArea);
    public void showArea();
    public int getAreaHeight();
    public int getAreaWidth();
    public JFrame getFrame();
}
