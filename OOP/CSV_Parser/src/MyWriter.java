import java.io.IOException;
import java.util.Map;

public abstract class MyWriter {
    public abstract void writeMap(Map<String, Integer> map, int wordsCount, String separateSymbol) throws IOException;
}
