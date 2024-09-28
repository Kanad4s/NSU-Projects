package ru.nsu.fit.tropin.View.TextView;

import ru.nsu.fit.tropin.Model.Resources;

import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.function.Function;

public class RecordTableText {
    private ArrayList<String[]> _players;

    public RecordTableText() {
        _players = getPreviousPlayers();
    }

    public void fillRecordTable() {
        for (int i = _players.size() - 1; i > 0; i--) {
            System.out.println(Arrays.toString(_players.get(i)));
        }
    }

    public void addPlayer(String name, int score) {
        if (name == null || name.isEmpty()) return;
        String[] player = new String[]{name, Integer.toString(score)};
        _players.add(player);
        _players.sort(Comparator.comparing(
                (Function<String[], String>) array->String.valueOf(Integer.parseInt(array[1]))).reversed());

        try (FileOutputStream fileOutputStream = new FileOutputStream(Resources.PATH_RECORD_TABLE);
             ObjectOutputStream oos = new ObjectOutputStream(fileOutputStream)) {
            oos.writeObject(_players);
        } catch (IOException e) {
            System.err.println("Result table not found");
        }
    }

    private ArrayList<String[]> getPreviousPlayers() {
        try (ObjectInputStream objectInputStream = new ObjectInputStream(new FileInputStream(Resources.PATH_RECORD_TABLE))) {
            _players = (ArrayList<String[]>) objectInputStream.readObject();
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
        return _players;
    }
}
