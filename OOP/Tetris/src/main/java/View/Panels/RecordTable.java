package View.Panels;

import Model.Resources;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.util.ArrayList;

public class RecordTable {
    private JTable _table;
    private JFrame _frame;
    private DefaultTableModel _tableModel;
    private ArrayList<String[]> _players;

    public RecordTable() {
        _frame = new JFrame(Resources.GAME_RULES_NAME);
        _tableModel = new DefaultTableModel();
        _players = getPreviousPlayers();
    }

    private ArrayList<String[]> getPreviousPlayers() {
        try (ObjectInputStream objectInputStream = new ObjectInputStream(new FileInputStream(Resources.RECORD_TABLE_NAME))) {
            _players = (ArrayList<String[]>) objectInputStream.readObject();
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
        return _players;
    }

    public void fillRecordTable() {
        _tableModel.setColumnIdentifiers(new String[]{"Player", "Score"});
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
}
