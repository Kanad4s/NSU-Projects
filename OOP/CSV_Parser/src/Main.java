
public class Main {
    final static int BUFFER_SIZE = 256;
    public static void main(String[] args) throws Exception {
        String inputFile = "test.txt";
        String outputFile = "out.csv";
        CSVParser parser = new CSVParser(BUFFER_SIZE);
        MyReader readerTXT = new ReaderTXT(inputFile);
        parser.setReader(readerTXT);
        parser.makeMap();
        //parser.printMap();
        MyWriter writerCSV = new WriterCSV(outputFile);
        parser.setWriter(writerCSV);
        parser.writeSortedMap();
    }
}
