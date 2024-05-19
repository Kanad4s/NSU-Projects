package ru.nsu.fit.tropin.Services.Parser;

import java.util.ArrayList;

public interface LineParser {
    public void parse(String line);
    public String getOperation();
    public ArrayList<String> getParameters();
}
