package ru.nsu.fit.tropin.Operations;

import ru.nsu.fit.tropin.Exceptions.OperationException;
import ru.nsu.fit.tropin.CalculatorController.CalculatorStack;
import org.springframework.stereotype.Component;

import java.util.ArrayList;
import java.util.Arrays;

import static java.lang.Math.sqrt;

@Component
public class Sqrt implements Operation {
    @Override
    public void calculation (CalculatorStack stack, ArrayList<String> listValues) throws Exception {
        LOGGER.debug("SQRT start");
        Double value;
        try {
            value = stack.peekValue();
        } catch (OperationException operationException) {
            LOGGER.warn("Stack is empty for SQRT" + operationException.getMessage());
            throw  new OperationException("Stack is empty" + Arrays.toString(operationException.getStackTrace()));
        }
        if (value < 0) {
            LOGGER.warn("Value is negative");
            throw new OperationException("The sqrt argument should: >= 0");
        }
        String pushValue = Double.valueOf(sqrt(value)).toString ();
        LOGGER.debug("Push SQRT result value "  + pushValue);
        stack.pushValue(pushValue);
    }
}
