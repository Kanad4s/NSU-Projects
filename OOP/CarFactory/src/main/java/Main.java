import ru.nsu.fit.tropin.GUI.FactoryFrame;
import ru.nsu.fit.tropin.GUI.GUIController;

import javax.swing.*;

public class Main {
    public static void main(String[] args) {
        GUIController gui_controller = new GUIController();
        SwingUtilities.invokeLater(() -> new FactoryFrame(gui_controller));
    }
}