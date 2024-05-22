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
            while (_isPlaying && _gameArea.isShapeMoving()) {
                try {
                    _gameArea.moveShapeDown();
                    Thread.sleep(Resources.DELAY);
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }

            }
            if (_isPlaying && _gameArea.isBlockOutOfBounds()) {
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
        _gameArea.createPlacedShapes();
        _gameArea.registerObserver(_gameAreaView);
        _gameArea.registerObserver(this);
        _movementController.setControl(_gameAreaView.getFrame(), _gameArea);
    }

    private void gameOver() {
        String playerName = _gameAreaView.getPlayerName();
        _gameAreaView.addPlayerToRecord(playerName, _gameArea.getPoints());
    }

    @Override
    public void update() {
        _isPlaying = true;
    }
}
