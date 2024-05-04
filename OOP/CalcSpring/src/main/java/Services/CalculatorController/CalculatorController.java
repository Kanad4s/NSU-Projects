package Services.CalculatorController;

import java.io.*;

public interface CalculatorController {
    public void launch() throws IOException, ClassNotFoundException;
    public void setInput(InputStream inputStream);
    public void setInput(String fileName) throws FileNotFoundException;
    public void test() throws IOException;
}
