import javax.print.DocFlavor;
import java.io.*;
import java.util.Arrays;

public class ReaderTXT extends Reader{
    private final int _bufferSize = 11;

    public ReaderTXT() throws FileNotFoundException {
    }
    @Override
    public int read(String fileName, byte[] buffer) {
        File inputFile = new File(fileName);
        int read = 0;
        try (InputStream inputStream = new FileInputStream(inputFile)) {
            BufferedInputStream bufferedInputStream = new BufferedInputStream(inputStream);
            read = bufferedInputStream.read(buffer);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
        return read;
    }

}
