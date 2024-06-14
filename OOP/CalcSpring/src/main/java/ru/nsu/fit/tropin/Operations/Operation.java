package ru.nsu.fit.tropin.Operations;

import ru.nsu.fit.tropin.CalculatorController.CalculatorStack;
import org.apache.log4j.Logger;

import java.util.ArrayList;

public interface Operation {
    public static final Logger LOGGER = Logger.getLogger(Operation.class.getName());
    public void calculation(CalculatorStack stack, ArrayList<String> listValues) throws Exception;
}
