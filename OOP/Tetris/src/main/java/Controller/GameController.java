package Controller;

import Model.Model;
import View.Panels.View;

public class GameController {
    private Model model;
    private View view;
    public GameController() {
        model = new Model();
        view = new View();
    }
    public void launch() {
        view.showMainMenu();
    }
}
