package Services.Operations;

import Exceptions.FormatDouble;
import Exceptions.OperationException;
import Services.CalculatorController.CalculatorStack;

import java.util.ArrayList;
import java.util.Arrays;

@CommandAnnotation
public class Division implements Operation {
    @Override
    public void calculation (CalculatorStack stack, ArrayList<String> listValues) throws Exception {
        try {
            Double v1 = stack.popValue();
            Double v2 = stack.popValue();
            if (v2 == 0) {
                throw new OperationException ("Division by zero");
            }
            LOGGER.debug("DIVISION values " + v1 + " and " + v2);
            Double result = v1 / v2;
            stack.pushValue(result.toString());
        } catch (OperationException operationException) {
            LOGGER.error("Stack is empty for DIVISION");
            throw new OperationException("Stack is empty" + Arrays.toString(operationException.getStackTrace()));
        }
    }
}
