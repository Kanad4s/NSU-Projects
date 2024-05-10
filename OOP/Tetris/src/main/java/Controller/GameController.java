package Controller;

import Model.Model;
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
        System.out.println("Game over");
        while (true) {

        }
    }

    private void launchGame() {
        gameAreaView.showArea();
        _movementController.setControl(gameAreaView.getFrame(), _model);
    }
}
