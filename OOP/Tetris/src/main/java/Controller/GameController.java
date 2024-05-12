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
            while (_model.isShapeMoving(gameAreaView.getFrame().getWidth() / 3, gameAreaView.getFrame().getHeight())) {
                try {
                    _model.moveShapeDown();
                    Thread.sleep(Resources.DELAY);
                    System.out.println("delay");
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }
            }
            System.out.println("Game over");
            _model.spawnNextShape(gameAreaView.getFrame().getWidth() / 3, gameAreaView.getFrame().getHeight());
        }
    }

    private void launchGame() {
        gameAreaView.showArea();
        _model.getSizes(gameAreaView.getFrame().getHeight(), gameAreaView.getFrame().getWidth() / 3);
        _model.registerObserver(gameAreaView);
        _movementController.setControl(gameAreaView.getFrame(), _model);
    }
}
