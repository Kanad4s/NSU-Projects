package Controller;

import View.MainMenu;

public class LaunchController {
    private MainMenu _view;
    public LaunchController() {};
    public void setMenu(MainMenu view) {
        _view = view;
    }
    public void launchMainMenu() {
        _view.showMainMenu();
    }
}
