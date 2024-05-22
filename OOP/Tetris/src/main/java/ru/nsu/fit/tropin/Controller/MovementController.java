package ru.nsu.fit.tropin.Controller;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

import static ru.nsu.fit.tropin.Model.Resources.*;
import ru.nsu.fit.tropin.Model.GameArea;
import ru.nsu.fit.tropin.View.GameAreaView;

public class MovementController {
    public MovementController() {};

    public void setControl(JFrame frame, GameArea gameArea) {
//        InputMap inputMap = frame.getRootPane().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW);
        InputMap inputMap = frame.getRootPane().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW);
        ActionMap actionMap = frame.getRootPane().getActionMap();
        inputMap.put(KeyStroke.getKeyStroke(KEY_UP), "up");
        inputMap.put(KeyStroke.getKeyStroke(KEY_DOWN), "down");
        inputMap.put(KeyStroke.getKeyStroke(KEY_LEFT), "left");
        inputMap.put(KeyStroke.getKeyStroke(KEY_RIGHT), "right");
        inputMap.put(KeyStroke.getKeyStroke(KEY_ROTATE), "rotate");
        actionMap.put("up", new AbstractAction() {
            public void actionPerformed(ActionEvent e) {
                gameArea.moveShapeUp();
            }
        });

        actionMap.put("down", new AbstractAction() {
            public void actionPerformed(ActionEvent e) {
                gameArea.moveShapeDown();
            }
        });

        actionMap.put("left", new AbstractAction() {
            public void actionPerformed(ActionEvent e) {
                gameArea.moveShapeLeft();
            }
        });

        actionMap.put("right", new AbstractAction() {
            public void actionPerformed(ActionEvent e) {
                gameArea.moveShapeRight();
            }
        });

        actionMap.put("rotate", new AbstractAction() {
            public void actionPerformed(ActionEvent e) {
                gameArea.moveShapeRotate();
            }
        });
    }

}
