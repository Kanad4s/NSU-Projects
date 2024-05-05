package View.Panels;

import Model.Resources;

import javax.swing.*;
import java.awt.*;

public class View {

    private JFrame _frame;
    private JPanel _panel;
    private JButton _startButton, _aboutButton, _exitButton, _scoreButton;

    public void showMainMenu() {
        createFrame();
        createPanel();
        _frame.setContentPane(_panel);
        _frame.setVisible(true);
        createButtons();
        showButtons();
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
                Image image = new ImageIcon(Resources.PATH_ASSETS + "Pattern01.png").getImage();
                g.drawImage(image, 0, 0, getWidth(), getHeight(), this);
            }
        };
        _panel.setOpaque(false);
    }

    private void createButtons() {
        createButtonAbout();
        createButtonStart();
        createButtonExit();
        createButtonScore();
    }

    private void createButtonStart() {
        _startButton = new JButton("Start");
        _startButton.addActionListener(e -> {

        });
    }

    private void createButtonAbout() {
        _aboutButton = new JButton("About");
        _aboutButton.addActionListener(e -> new GameRules());
    }

    private void createButtonScore() {
        _scoreButton = new JButton("Score");
    }

    private void createButtonExit() {
        _exitButton = new JButton("Exit");
    }

    private void showButtons() {
        _frame.getContentPane().add(_startButton);
        _frame.getContentPane().add(_aboutButton);
        _frame.getContentPane().add(_scoreButton);
        _frame.getContentPane().add(_exitButton);
        _frame.setLayout(new ButtonsLayout());
    }
}
