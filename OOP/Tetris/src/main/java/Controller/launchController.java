package Controller;

import View.Panels.MainMenu;

public class launchController {
    private MainMenu view;
    public launchController() {
        view = new MainMenu();
    }
    public void launchMainMenu() {
        view.showMainMenu();
    }
}
