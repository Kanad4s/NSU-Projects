package ru.nsu.fit.tropin.Operations;

import ru.nsu.fit.tropin.Exceptions.OperationException;
import ru.nsu.fit.tropin.CalculatorController.CalculatorStack;
import org.springframework.stereotype.Component;

import java.util.ArrayList;
import java.util.Arrays;


@Component
public class Plus implements Operation{
    public Plus() {}

    @Override
    public void calculation(CalculatorStack stack, ArrayList<String> listValues) throws Exception {
        try {
            Double v1 = stack.popValue();
            Double v2 = stack.popValue();
            LOGGER.debug("PLUS values " + v1 + " and " + v2);
            Double result = v1 + v2;
            stack.pushValue(result.toString());
        } catch (OperationException operationException) {
            LOGGER.error("Stack is empty for PLUS");
            throw new OperationException("Stack is empty" + Arrays.toString(operationException.getStackTrace()));
        }
    }
}
