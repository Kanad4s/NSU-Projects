package ru.nsu.fit.tropin.Services.Parser.CmdParser;

import java.io.FileNotFoundException;
import java.io.InputStreamReader;

public interface CmdParser {
    public boolean parse(String[] args);
    public InputStreamReader getInputStream() throws FileNotFoundException;
}
