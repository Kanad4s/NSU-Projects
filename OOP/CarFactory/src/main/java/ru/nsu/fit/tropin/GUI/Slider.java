package ru.nsu.fit.tropin.GUI;
import javax.swing.*;

public class Slider extends JSlider{
    public final static int SEC = 1000;
    public static final int FPS_MIN = 1;
    public static final int FPS_MAX = 31;
    public static final int FPS_INIT = 16;

    public Slider(){
        super(JSlider.HORIZONTAL,
                FPS_MIN, FPS_MAX, FPS_INIT);
        this.setMajorTickSpacing(5);
        this.setMinorTickSpacing(0);
        this.setPaintTicks(true);
        this.setPaintLabels(true);

    }
}
