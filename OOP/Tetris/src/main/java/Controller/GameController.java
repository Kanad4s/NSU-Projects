package Controller;

import Model.Model;
import Model.MyObserver;
import Model.Resources;
import View.Panels.GameAreaView;
import View.Panels.RecordTable;
import javax.swing.*;

public class GameController implements Runnable, MyObserver {
    private final GameAreaView gameAreaView;
    private final MovementController _movementController;
    private final Model _model;
    private boolean _isPlaying = false;

    public GameController(){
        _model = new Model();
        gameAreaView = new GameAreaView(_model);
        _movementController = new MovementController();
    }

    @Override
    public void run() {
        launchGame();
        while (true) {
            while (_model.isShapeMoving() && _isPlaying) {
                try {
                    _model.moveShapeDown();
                    Thread.sleep(Resources.DELAY);
                    System.out.println("delay");
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }
            }
            if (_model.isBlockOutOfBounds() && _isPlaying) {
                System.out.println("Game Over");
                Thread.currentThread().interrupt();
                _isPlaying = false;
                gameOver();
            }
            if (_isPlaying) {
                System.out.println("New shape");
                _model.spawnNextShape();
            }
        }
    }

    private void launchGame() {
        _isPlaying = true;
        gameAreaView.showArea();
        _model.setAreaSize(gameAreaView.getFrame().getHeight(), gameAreaView.getFrame().getWidth());
        _model.createPlacedShapes();
        _model.registerObserver(gameAreaView);
        _model.registerObserver(this);
        _movementController.setControl(gameAreaView.getFrame(), _model);
    }

    private void gameOver() {
        String playerName = JOptionPane.showInputDialog("Game Over\n Please, input your name.");
        addPlayerToRecord(playerName, _model.getPoints());
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
