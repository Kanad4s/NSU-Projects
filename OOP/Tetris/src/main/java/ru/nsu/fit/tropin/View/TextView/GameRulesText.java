package ru.nsu.fit.tropin.View.TextView;

import ru.nsu.fit.tropin.Model.Shape.*;

public class GameRulesText {
    public GameRulesText() {
        addRulesText();
        addFigures();
        MainMenuText.showButtons();
    }

    private void addRulesText() {
        System.out.println("Tetris\n" +
                "Use arrow keys to move the falling blocks and try to fill\n" +
                "complete rows to score points. The game ends when the blocks\n" +
                "reach the top of the screen.The main goal of the game is to score as many points as possible\n" +
                "POINTS can be obtained for arranging pieces +10 and if a line of pieces +100 is built.\n" +
                "The game has 3 modes: LIGHT, MEDIUM, HARD. By default set to MEDIUM.\n" +
                "The game has the following figures:");
    }

    private void addFigures() {
        int figuresLength = new Hero().getWidth() + new Rhode().getWidth() + new Ricky().getWidth()
                + new Smashboy().getWidth() + new Teewee().getWidth() + 4;
        System.out.println("     | ** | *   | ** |  * \n" +
                           "**** | ** | *** | ** | ***");
    }
}
