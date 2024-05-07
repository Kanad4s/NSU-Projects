package Controller;

import javax.swing.*;
import java.awt.event.ActionEvent;

import static Model.Resources.*;
import Model.Model;
//import static Model.Resources.KEY_UP;

public class MovementController {
    public MovementController(JFrame frame, Model model) {
        InputMap inputMap = frame.getRootPane().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW);
        ActionMap actionMap = frame.getRootPane().getActionMap();
        inputMap.put(KeyStroke.getKeyStroke(KEY_UP), "up");
        inputMap.put(KeyStroke.getKeyStroke(KEY_DOWN), "down");
        inputMap.put(KeyStroke.getKeyStroke(KEY_LEFT), "left");
        inputMap.put(KeyStroke.getKeyStroke(KEY_RIGHT), "right");
        inputMap.put(KeyStroke.getKeyStroke(KEY_ROTATE), "rotate");
        actionMap.put("up", new AbstractAction() {
            public void actionPerformed(ActionEvent e) {
                model.moveShapeUp();
            }
        });

        actionMap.put("down", new AbstractAction() {
            public void actionPerformed(ActionEvent e) {
                model.moveShapeDown();
            }
        });

        actionMap.put("left", new AbstractAction() {
            public void actionPerformed(ActionEvent e) {
                model.moveShapeLeft();
            }
        });

        actionMap.put("right", new AbstractAction() {
            public void actionPerformed(ActionEvent e) {
                model.moveShapeRight();
            }
        });

        actionMap.put("rotate", new AbstractAction() {
            public void actionPerformed(ActionEvent e) {
                model.moveShapeRotate();
            }
        });
    }
}
