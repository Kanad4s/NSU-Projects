public class CommandLineParser {
    private String _inputFile;
    private String _outputFile;

    public void parse(String[] args) {
        _inputFile = args[0];
        _outputFile = args[1];
    }

    public String getInputFile() {
        return _inputFile;
    }

    public String getOutputFile() {
        return _outputFile;
    }
}
