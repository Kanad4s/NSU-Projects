package ru.nsu.fit.tropin.View.GView;

import ru.nsu.fit.tropin.Model.Resources;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.io.*;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.function.Function;

public class RecordTable {
    private JTable _table;
    private JFrame _frame;
    private DefaultTableModel _tableModel;
    private ArrayList<String[]> _players;

    public RecordTable() {
        _frame = new JFrame(Resources.GAME_RULES_NAME);
        _tableModel = new DefaultTableModel();
        _tableModel.addColumn("Player");
        _tableModel.addColumn("Points");
        _players = getPreviousPlayers();
    }

    public void fillRecordTable() {
        for (String[] player : _players) {
            _tableModel.addRow(player);
        }
        _table = new JTable(_tableModel);
        JScrollPane scrollPane = new JScrollPane(_table);
        _frame.getContentPane().add(scrollPane, BorderLayout.CENTER);
        _frame.pack();
        _frame.setLocationRelativeTo(null);
        _frame.setVisible(true);
    }

    public void addPlayer(String name, int score) {
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
