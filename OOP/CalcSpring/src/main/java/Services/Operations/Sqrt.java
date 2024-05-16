package Services.Operations;

import Exceptions.OperationException;
import Services.CalculatorController.CalculatorStack;

import java.util.ArrayList;
import java.util.Arrays;

import static java.lang.Math.sqrt;

@CommandAnnotation
public class Sqrt implements Operation {
    @Override
    public void calculation (CalculatorStack stack, ArrayList<String> listValues) throws Exception {
        LOGGER.debug("SQRT start");
        Double value;
        try {
            value = stack.peekValue();
            if (value < 0) {
                throw new OperationException("The sqrt argument should: >= 0");
            }
        } catch (OperationException operationException) {
            LOGGER.warn("Stack is empty for SQRT" + operationException.getMessage());
            throw  new OperationException("Stack is empty" + Arrays.toString(operationException.getStackTrace()));
        }
        String pushValue = Double.valueOf(sqrt(value)).toString ();
        LOGGER.debug("Push SQRT result value "  + pushValue);
        stack.pushValue(pushValue);
    }
}
