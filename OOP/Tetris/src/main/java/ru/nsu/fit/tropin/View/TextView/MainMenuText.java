package ru.nsu.fit.tropin.View.TextView;

import ru.nsu.fit.tropin.Controller.GameController;
import ru.nsu.fit.tropin.View.MainMenu;
import java.util.Scanner;

public class MainMenuText implements MainMenu {

    @Override
    public void showMainMenu() {
        showButtons();
        String input;
        boolean isParsed = true;
        while (isParsed) {
            input = readInput();
            isParsed = parseInput(input);
            if (isParsed) showButtons();
        }
    }

    public static void showButtons() {
        System.out.println("1. Play\n2. Rules\n3. Score\n4. Exit");
    }

    private String readInput() {
        Scanner in = new Scanner(System.in);
        return in.nextLine();
    }

    private boolean parseInput(String input) {
        switch (input) {
            case "1" -> {
                //play
                GameController gameController = new GameController();
                gameController.setGameAreaView(new GameAreaViewText());
                gameController.run();
                return false;
            }
            case "2" -> {
                new GameRulesText();
                return true;
            }
            case "3" -> {
                RecordTableText recordTableText = new RecordTableText();
                recordTableText.fillRecordTable();
                return true;
            }
            case "4" -> {
                System.exit(0);
            }
            case "5" -> {
                cls();
//                System.out.println("\f");
//                System.out.flush();
            }

            case null, default -> {
                System.out.println("Invalid input");
            }
        }
        return true;
    }

    private void cls() {
        try {
//            new ProcessBuilder("cmd", "/c", "cls").inheritIO().start().waitFor();
            Runtime.getRuntime().exec("clear");
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }
}
