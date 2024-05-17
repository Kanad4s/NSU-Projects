package org.Application;

import Controller.LaunchController;
import View.GView.MainMenuGraphic;
import View.TextView.MainMenuText;

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