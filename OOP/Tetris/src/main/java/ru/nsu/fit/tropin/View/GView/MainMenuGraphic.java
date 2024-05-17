package ru.nsu.fit.tropin.View.GView;

import ru.nsu.fit.tropin.Controller.GameController;
import ru.nsu.fit.tropin.Model.Resources;
import ru.nsu.fit.tropin.View.MainMenu;

import javax.swing.*;
import java.awt.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class MainMenuGraphic implements MainMenu {

    private JFrame _frame;
    private JPanel _panel;
    private JButton _startButton, _aboutButton, _exitButton, _scoreButton;

    @Override
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
        //_panel.setOpaque(true);
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
            _frame.dispose();
            ExecutorService executorService = Executors.newSingleThreadExecutor();
            GameController gameController = new GameController();
            gameController.setGameAreaView(new GameAreaViewGraphic());
            executorService.execute(gameController);
            executorService.shutdown();
        });
    }

    private void createButtonAbout() {
        _aboutButton = new JButton("About");
        _aboutButton.addActionListener(e -> new GameRulesGraphic());
    }

    private void createButtonScore() {
        _scoreButton = new JButton("Score");
        _scoreButton.addActionListener(e -> {
            RecordTable recordTable = new RecordTable();
            recordTable.fillRecordTable();
        });
    }

    private void createButtonExit() {
        _exitButton = new JButton("Exit");
        _exitButton.addActionListener(e -> System.exit(0));
    }

    private void showButtons() {
        _frame.getContentPane().add(_startButton);
        _frame.getContentPane().add(_aboutButton);
        _frame.getContentPane().add(_scoreButton);
        _frame.getContentPane().add(_exitButton);
        _frame.setLayout(new ButtonsMenuLayout());
    }
}
