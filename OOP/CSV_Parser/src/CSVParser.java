import java.io.FileNotFoundException;
import java.io.IOException;

public class CSVParser {
    private final static String SEPARATE_SYMBOL = ";";
    private final byte[] _buffer;
    private final MapWords _mapWords;
    private MyReader _reader;
    private MyWriter _writer;

    public CSVParser(int bufferSize) throws FileNotFoundException {
        _buffer = new byte[bufferSize];
        _mapWords = new MapWords();
    }
    public void setReader(MyReader reader) {
        _reader = reader;
    }
    public void setWriter(MyWriter writer) {
        _writer = writer;
    }
    public void makeMap() throws Exception {
        int read = _reader.read(_buffer);
        while (read != -1) {
            String line = new String(_buffer, 0, read);
            _mapWords.extendMap(line, read);
            read = _reader.read(_buffer);
        }
    }
    public void writeSortedMap() throws IOException {
        _writer.writeMap(_mapWords.getSortedMap(), _mapWords.getWordsCount(), SEPARATE_SYMBOL);
    }
    public void printMap(){
        _mapWords.printMap();
    }
}
