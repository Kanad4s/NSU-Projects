import java.io.FileNotFoundException;

public class Main {
    final static int BUFFER_SIZE = 11;

    public static void main(String[] args) throws FileNotFoundException {
        System.out.printf("Hello and welcome!\n");
        String inputFile = "test.txt";
        CSVParser parser = new CSVParser(BUFFER_SIZE);
        Reader readerTXT = new ReaderTXT();
        parser.setReader(readerTXT);
        parser.makeMap();
        //parser.read(readerTXT, inputFile);
    }
}