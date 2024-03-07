import java.io.FileNotFoundException;

public class CSVParser {
    private String _inputFile = "test.txt";
    private byte[] _buffer;
    private MapWords _mapWords;
    private Reader _reader;

    public CSVParser(int bufferSize) throws FileNotFoundException {
        _buffer = new byte[bufferSize];
        _mapWords = new MapWords();
    }

    public void setReader(Reader reader) {
        _reader = reader;
    }

    public void makeMap() {
        int read = 0;
        read = _reader.read(_inputFile, _buffer);
        String s = new String(_buffer, 0, read);
        System.out.print(s);
        while (read != 0) {
            StringBuilder line = new StringBuilder(new String(_buffer, 0, read));
            _mapWords.extendMap(line, read);
            read = _reader.read(_inputFile, _buffer);
            System.out.println(new String(_buffer, 0, read));
        }
    }
    public void read(Reader reader, String inputFile) {
        int read = reader.read(inputFile, _buffer);
        String s = new String(_buffer, 0, read);
        System.out.println(s);
    }
}
