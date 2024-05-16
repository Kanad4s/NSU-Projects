package org.Application;

import Controller.launchController;

public class Main {
    public static void main(String[] args) {
        try {
            launchController gameController = new launchController();
            gameController.launchMainMenu();
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }
}