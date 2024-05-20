package ru.nsu.fit.tropin;

import ru.nsu.fit.tropin.Controller.LaunchController;
import ru.nsu.fit.tropin.Parser.CommandLineParser;
import ru.nsu.fit.tropin.View.GView.MainMenuGraphic;
import ru.nsu.fit.tropin.View.TextView.MainMenuText;

public class Main {
    public static void main(String[] args) {
        LaunchController launchController = new LaunchController();
        CommandLineParser parser = new CommandLineParser();
        if (parser.parse(args)) {
            launchController.setMenu(parser.getMainMenu());
            try {
                launchController.launchMainMenu();
            } catch (Exception e) {
                System.out.println(e.getMessage());
            }
        }
    }
}