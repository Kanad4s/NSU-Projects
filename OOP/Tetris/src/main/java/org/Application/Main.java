package org.Application;

import Controller.GameController;

public class Main {
    public static void main(String[] args) {
        GameController gameController = new GameController();
        try {
            gameController.launch();
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }
}