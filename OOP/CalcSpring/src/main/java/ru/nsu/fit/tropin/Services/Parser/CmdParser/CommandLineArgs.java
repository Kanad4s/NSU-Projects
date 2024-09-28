package ru.nsu.fit.tropin.Services.Parser.CmdParser;

import com.beust.jcommander.Parameter;

public class CommandLineArgs {
    @Parameter(
            names = {"--file", "-f"},
            description = "input stream"
    )
    private String file;

    @Parameter(
            names = {"--help", "-h"},
            description = "possible options",
            help = true
    )
    private boolean help;

    public String getFile() {
        return file;
    }

    public boolean isHelp() {
        return help;
    }
}


