package Services.Operations;

import Exceptions.ArgumentsNumberException;
import Exceptions.FormatDouble;
import Services.CalculatorController.CalculatorStack;
import Services.InputChecker.InputChecker;

import java.util.ArrayList;

@CommandAnnotation
public class Push implements Operation{

    @Override
    public void calculation (CalculatorStack stack, ArrayList<String> listValues) throws Exception {
        if (listValues.size() == 1) {
            LOGGER.debug ("Push value in stack");
            if (InputChecker.isDouble(listValues.get(0))) {
                stack.pushValue(listValues.get(0));
            } else if (stack.getMapDefineValues().containsKey(listValues.get(0))) {
                stack.pushValue(String.valueOf(stack.getDefineValue(listValues.get(0))));
            } else {
                throw new FormatDouble("Defined value not found");
            }
        } else {
            LOGGER.error ("Wrong number of PUSH arguments");
            throw new ArgumentsNumberException("Wrong number of arguments ");
        }
    }
}
