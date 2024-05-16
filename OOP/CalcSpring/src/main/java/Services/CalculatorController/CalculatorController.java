package Services.CalculatorController;

import Services.Operations.Operation;

import java.io.*;

public interface CalculatorController {
    public void launch() throws IOException, ClassNotFoundException, InstantiationException, IllegalAccessException;
    public void setInput(InputStream inputStream);
    public void setInput(String fileName) throws FileNotFoundException;
}
