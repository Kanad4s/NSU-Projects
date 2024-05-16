package Services.Operations;

import Exceptions.OperationException;
import Services.CalculatorController.CalculatorStack;

import java.util.ArrayList;
import java.util.Arrays;

@CommandAnnotation
public class Multiplication implements Operation {
    @Override
    public void calculation(CalculatorStack stack, ArrayList<String> listValues) throws Exception {
        try {
            Double v1 = stack.popValue();
            Double v2 = stack.popValue();
            LOGGER.debug("MULTIPLICATION values " + v1 + " and " + v2);
            Double result = v1 * v2;
            stack.pushValue(result.toString());
        } catch (OperationException operationException) {
            LOGGER.error("Stack is empty for MULTIPLICATION");
            throw new OperationException("Stack is empty" + Arrays.toString(operationException.getStackTrace()));
        }
    }
}
