package ru.nsu.fit.tropin.Controller;

import ru.nsu.fit.tropin.Model.GameArea;
import ru.nsu.fit.tropin.Model.MyObserver;
import ru.nsu.fit.tropin.Model.Resources;
import ru.nsu.fit.tropin.View.GView.RecordTable;
import ru.nsu.fit.tropin.View.GameAreaView;

import javax.swing.*;

public class GameController implements Runnable, MyObserver {
    private GameAreaView _gameAreaView;
    private final MovementController _movementController;
    private final GameArea _gameArea;
    private boolean _isPlaying = false;

    public GameController(){
        _gameArea = new GameArea();
//        _gameAreaView = new GameAreaViewGraphic(_gameArea);
        _movementController = new MovementController();
    }

    public void setGameAreaView(GameAreaView gameAreaView) {
        _gameAreaView = gameAreaView;
        _gameAreaView.setGameArea(_gameArea);
    }

    @Override
    public void run() {
        launchGame();
        while (true) {
            while (_gameArea.isShapeMoving() && _isPlaying) {
                try {
                    _gameArea.moveShapeDown();
                    Thread.sleep(Resources.DELAY);
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }

            }
            if (_gameArea.isBlockOutOfBounds() && _isPlaying) {
//                System.out.println(_gameArea.isBlockOutOfBounds());
//                System.out.println(_gameArea.getPoints());
//                System.out.println(_isPlaying);
                Thread.currentThread().interrupt();
                _isPlaying = false;
                gameOver();
            }
            if (_isPlaying) {
                _gameArea.spawnNextShape();
            }
        }
    }

    private void launchGame() {
        _isPlaying = true;
        _gameAreaView.showArea();
        _gameArea.setAreaSize(_gameAreaView.getAreaHeight(), _gameAreaView.getAreaWidth());
//        _gameArea.setAreaSize(gameAreaViewGraphic.getFrame().getHeight(), gameAreaViewGraphic.getFrame().getWidth());
        _gameArea.createPlacedShapes();
        _gameArea.registerObserver(_gameAreaView);
        _gameArea.registerObserver(this);
        _movementController.setControl(_gameAreaView.getFrame(), _gameArea);
    }

    private void gameOver() {
        String playerName = JOptionPane.showInputDialog("Game Over\n Please, input your name.");
        addPlayerToRecord(playerName, _gameArea.getPoints());
    }

    private void addPlayerToRecord(String name, int points) {
        RecordTable recordTable = new RecordTable();
        recordTable.addPlayer(name, points);
        recordTable.fillRecordTable();
    }

    @Override
    public void update() {
        _isPlaying = true;
    }
}
