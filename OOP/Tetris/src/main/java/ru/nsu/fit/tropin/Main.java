package ru.nsu.fit.tropin;

import ru.nsu.fit.tropin.Controller.LaunchController;
import ru.nsu.fit.tropin.View.GView.MainMenuGraphic;
import ru.nsu.fit.tropin.View.TextView.MainMenuText;

public class Main {
    public static void main(String[] args) {
        LaunchController launchController = new LaunchController();
        if (args.length == 0) {
            launchController.setMenu(new MainMenuText());
        } else {
            launchController.setMenu(new MainMenuGraphic());
        }
        try {
            launchController.launchMainMenu();
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }
}