package ru.nsu.fit.tropin.Controller;

import ru.nsu.fit.tropin.Model.GameArea;
import ru.nsu.fit.tropin.Model.MyObserver;
import ru.nsu.fit.tropin.Model.Resources;
import ru.nsu.fit.tropin.View.GameAreaView;

public class GameController implements Runnable, MyObserver {
    private GameAreaView _gameAreaView;
    private final MovementController _movementController;
    private final GameArea _gameArea;
    private boolean _isPlaying = false;
    private boolean _isShutdown = false;

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
        while (!_isShutdown) {
            while (_isPlaying && _gameArea.isShapeMoving()) {
                try {
                    _gameArea.moveShapeDown();
                    Thread.sleep(Resources.DELAY);
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }

            }
            waitBlock();
            if (_isPlaying && _gameArea.isBlockOutOfBounds()) {
                System.out.println("block out of bounds");
                Thread.currentThread().interrupt();
                _isPlaying = false;
                gameOver();
            }
            if (_isPlaying) {
                _gameArea.spawnNextShape();
            }
        }
    }

    private void waitBlock() {
        try {
            Thread.sleep(1);
        } catch (InterruptedException e) {
            System.out.println("interrupted");
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
        System.out.println("Game Over!\nEnter player name:");
        String playerName = _gameAreaView.getPlayerName();
        System.out.println(playerName + " score: " + _gameArea.getPoints());
        _gameAreaView.addPlayerToRecord(playerName, _gameArea.getPoints());
    }

    @Override
    public void update() {
        if (!_isPlaying && _gameArea.getPoints() == 0) {
            System.out.println("Set playing");
            _isPlaying = true;
        }
    }

    public void setShutdown(boolean status) {
        _isShutdown = status;
    }

    public void setPlaying(boolean playing) {
        _isPlaying = playing;
    }
}
