package ru.nsu.fit.tropin.CalculatorController;

import java.io.*;

public interface CalculatorController {
    public void launch() throws Exception;
    public void setInput(InputStream inputStream);
    public void setInput(String fileName) throws FileNotFoundException;
}
