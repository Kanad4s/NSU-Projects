package View.GView;

import Model.Resources;

import javax.swing.*;
import java.awt.*;

public class GameRules {
    private JFrame _frame;
    private JPanel _panel;
    public GameRules() {
        _frame = new JFrame(Resources.GAME_RULES_NAME);
        _panel = new JPanel(new BorderLayout());
        addRulesText();
        addFigures();
        _frame.setContentPane(_panel);
        _frame.pack();
        _frame.setLocationRelativeTo(null);
        _frame.setVisible(true);
    }

    private void addRulesText() {
        JLabel title = new JLabel("<html><h1>Tetris</h1>" +
                "<p>Use arrow keys to move the falling blocks and try to fill " +
                "complete rows to score points. The game ends when the blocks " +
                "reach the top of the screen.The main goal of the game is to score as many points as possible <br> <br> " +
                "POINTS can be obtained for arranging pieces +10 and if a line of pieces +100 is built." +
                "The game has 3 modes: LIGHT, MEDIUM, HARD. By default set to MEDIUM." +
                "The game has the following figures:</p></html>");
        _panel.add(title, BorderLayout.NORTH);
    }

    private void addFigures() {
        JPanel figuresPanel = new JPanel(new GridLayout(1, 5));
        ImageIcon heroIcon = new ImageIcon(Resources.PATH_HERO);
        ImageIcon rhodeIcon = new ImageIcon(Resources.PATH_RHODE);
        ImageIcon rickyIcon = new ImageIcon(Resources.PATH_RICKY);
        ImageIcon teeweeIcon = new ImageIcon(Resources.PATH_TEEWEE);
        ImageIcon smashboyIcon = new ImageIcon(Resources.PATH_SMASHBOY);

        figuresPanel.add(new JLabel(heroIcon));
        figuresPanel.add(new JLabel(rhodeIcon));
        figuresPanel.add(new JLabel(rickyIcon));
        figuresPanel.add(new JLabel(teeweeIcon));
        figuresPanel.add(new JLabel(smashboyIcon));

        _panel.add(figuresPanel, BorderLayout.SOUTH);
    }
}
