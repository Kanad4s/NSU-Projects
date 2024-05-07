package Controller;

import Model.Model;
import View.Panels.GameAreaView;

public class GameController implements Runnable{
    private GameAreaView gameAreaView;
    private MovementController _movementController;
    private Model _model;

    public GameController(){
        gameAreaView = new GameAreaView();
        _model = new Model();
        _movementController = new MovementController(gameAreaView.getFrame(), _model);
    }

    @Override
    public void run() {
        launchGame();
        while (true) {

        }
    }

    private void launchGame() {
        gameAreaView.showArea();
    }
}
