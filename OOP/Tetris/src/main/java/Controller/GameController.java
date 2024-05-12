package Controller;

import Model.Model;
import Model.Resources;
import View.Panels.GameAreaView;

public class GameController implements Runnable{
    private GameAreaView gameAreaView;
    private MovementController _movementController;
    private Model _model;

    public GameController(){
        _model = new Model();
        gameAreaView = new GameAreaView(_model);
        _movementController = new MovementController();
    }

    @Override
    public void run() {
        launchGame();
        while (true) {
            while (_model.isShapeMoving()) {
                try {
                    _model.moveShapeDown();
                    Thread.sleep(Resources.DELAY);
                    System.out.println("delay");
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }
            }

            if (_model.isBlockOutOfBounds()) {
                System.out.println("Game Over");
                Thread.currentThread().interrupt();
                //PerformanceGameArea.gameOver();
            }

            System.out.println("New shape");
            _model.spawnNextShape();
        }
    }

    private void launchGame() {
        gameAreaView.showArea();
        _model.setAreaSize(gameAreaView.getFrame().getHeight(), gameAreaView.getFrame().getWidth());
        _model.createPlacedShapes();
        _model.registerObserver(gameAreaView);
        _movementController.setControl(gameAreaView.getFrame(), _model);
    }
}
