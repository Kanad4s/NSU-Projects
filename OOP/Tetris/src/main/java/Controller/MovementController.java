package Controller;

import javax.swing.*;
import java.awt.event.ActionEvent;

import static Model.Resources.*;
import Model.GameArea;
import View.GameAreaView;

public class MovementController {
    public MovementController() {};

    public void setControl(GameAreaView gameAreaView, GameArea gameArea) {
        JFrame frame = gameAreaView.getFrame();
//        InputMap inputMap = frame.getRootPane().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW);
        InputMap inputMap = frame.getRootPane().getInputMap();
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
