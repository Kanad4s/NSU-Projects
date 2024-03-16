import java.util.*;
import java.util.stream.Collectors;

public class MapWords {
    private final static String ALLOWED_SYMBOLS_REGEX = "[^a-zA-Zа-яА-Я`0-9]+";
    private HashMap<String, Integer> map;
    private int _wordsCount;
    public MapWords() {
        map = new HashMap<>();
    }

    public void extendMap(String line, int size) {
        String[] words = line.split(ALLOWED_SYMBOLS_REGEX);
        for (int i = 0; i < words.length; i++) {
            int value;
            if (map.containsKey(words[i])) {
                value = map.get(words[i]) + 1;
            } else {
                value = 1;
            }
            map.put(words[i], value);
        }
        _wordsCount += words.length;
    }

    public void printMap() {
        map.entrySet().forEach(System.out::println);
    }

    public Map<String, Integer> getSortedMap() {
        return map.entrySet()
                .stream()
                .sorted(Map.Entry.comparingByValue(Comparator.reverseOrder()))
                .collect(Collectors.toMap(
                        Map.Entry::getKey,
                        Map.Entry::getValue,
                        (oldValue, newValue) -> oldValue, LinkedHashMap::new));
    }

    public int getWordsCount() {
        return _wordsCount;
    }
}
