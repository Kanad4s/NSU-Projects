
public class Main {
    //javac Main.java
    //java Main.java
    final static int BUFFER_SIZE = 4096;
    public static void main(String[] args) throws Exception {
        try {
            CommandLineParser commandLineParser = new CommandLineParser();
            commandLineParser.parse(args);
            String inputFile = commandLineParser.getInputFile();
            String outputFile = commandLineParser.getOutputFile();
            CSVParser parser = new CSVParser(BUFFER_SIZE);
            parser.setReader(new ReaderTXT(inputFile));
            parser.makeMap();
            parser.setWriter(new WriterCSV(outputFile));
            parser.writeSortedMap();
        } catch (Exception e) {
            throw new Exception(e);
        }
    }
}
