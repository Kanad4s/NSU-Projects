package Services.CalculatorController;

import java.io.*;

public class CalculatorController {
//    private Reader _reader;
    private BufferedReader _br;
    private String _line;

    public CalculatorController() {
        _br = new BufferedReader(new BufferedReader(new InputStreamReader(System.in)));
    }

    public CalculatorController(String fileName) throws FileNotFoundException {
        _br = new BufferedReader(new InputStreamReader(new FileInputStream(fileName)));
    }

    public void launch() throws IOException {
        while ((_line = _br.readLine()) != null) {

        }
    }


}
