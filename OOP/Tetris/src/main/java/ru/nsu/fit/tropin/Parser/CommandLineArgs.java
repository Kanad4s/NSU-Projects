package ru.nsu.fit.tropin.Parser;

import com.beust.jcommander.Parameter;

public class CommandLineArgs {
    @Parameter(
            names = {"-v", "--view"},
            description = "graphic interface"
    )
    private boolean view;

    @Parameter(
            names = {"--help", "-h"},
            description = "possible options",
            help = true
    )
    private boolean help;

    public boolean isHelp() {
        return help;
    }

    public boolean isView() {
        return view;
    }
}
