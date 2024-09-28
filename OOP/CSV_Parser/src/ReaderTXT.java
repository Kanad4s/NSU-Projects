import java.io.*;

public class ReaderTXT extends MyReader {
    private final BufferedInputStream bufferedInputStream;
    public ReaderTXT(String fileName) throws FileNotFoundException {
        bufferedInputStream = new BufferedInputStream(new FileInputStream(fileName));
    }
    @Override
    public int read(byte[] buffer) throws Exception {
        int read = 0;
        read = bufferedInputStream.read(buffer);
        return read;
    }

}
