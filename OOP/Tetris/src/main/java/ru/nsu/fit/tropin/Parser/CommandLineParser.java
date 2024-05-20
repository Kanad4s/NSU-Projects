package ru.nsu.fit.tropin.Parser;


import com.beust.jcommander.JCommander;
import ru.nsu.fit.tropin.View.GView.MainMenuGraphic;
import ru.nsu.fit.tropin.View.MainMenu;
import ru.nsu.fit.tropin.View.TextView.MainMenuText;

public class CommandLineParser {
    JCommander jCommander;
    private final CommandLineArgs _commandLineArgs;

    public CommandLineParser() {
        _commandLineArgs = new CommandLineArgs();
    }

    public boolean parse(String[] args) {
        jCommander = new JCommander(_commandLineArgs);
        jCommander.parse(args);
        System.out.println(_commandLineArgs.isView());
        if (_commandLineArgs.isHelp()) {
            jCommander.usage();
            return false;
        }
        return true;
    }

    public MainMenu getMainMenu() {
        if (_commandLineArgs.isView()) {
            return new MainMenuGraphic();
        } else {
            return new MainMenuText();
        }
    }

}
