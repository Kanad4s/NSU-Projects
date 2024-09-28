package ru.nsu.fit.tropin.Services.Parser.CmdParser;

import com.beust.jcommander.JCommander;
import org.apache.log4j.Logger;
import org.springframework.stereotype.Component;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStreamReader;

@Component
public class CommandLineParser implements CmdParser {
    private static final Logger LOGGER = Logger.getLogger(CommandLineParser.class);
    private final CommandLineArgs _commandLineArgs;

    public CommandLineParser() {
        _commandLineArgs = new CommandLineArgs();
    }

    @Override
    public boolean parse(String[] args) {
        LOGGER.info("Starting command line parser");
        JCommander _jCommander = new JCommander(_commandLineArgs);
        _jCommander.parse(args);
        if (_commandLineArgs.isHelp()) {
            _jCommander.usage();
            return false;
        }
        return true;
    }

    @Override
    public InputStreamReader getInputStream() throws FileNotFoundException {
        if (_commandLineArgs.getFile() == null) {
            LOGGER.info("Set console input stream");
            return new InputStreamReader(System.in);
        } else {
            LOGGER.info("Set file input stream");
            return new InputStreamReader(new FileInputStream(_commandLineArgs.getFile()));
        }
    }
}
