import java.io.*;
import java.util.*;

public class WriterCSV extends MyWriter{
    private final BufferedOutputStream bufferedOutputStream;
    public WriterCSV(String fileName) throws FileNotFoundException {
        bufferedOutputStream = new BufferedOutputStream(new FileOutputStream(fileName));
    }
    @Override
    public void writeMap(Map<String, Integer> map, int wordsCount, String separateSymbol) throws IOException {
        Object[] keys = map.keySet().toArray();
        ArrayList<Integer> value = new ArrayList<>(map.values());
        bufferedOutputStream.write(getUpperLine(separateSymbol).getBytes());
        for (int i = 0; i < map.size(); i++) {
            String line = keys[i] + separateSymbol +
                    value.get(i) + separateSymbol +
                    100 * (float)value.get(i) / wordsCount + "\n";
            bufferedOutputStream.write(line.getBytes("windows-1251"));
            bufferedOutputStream.flush();
            //System.out.print(line);
        }
    }

    private String getUpperLine(String separateSymbol) {
        return "Key" + separateSymbol + "Value" + separateSymbol + "PercentCount\n";
    }
}
