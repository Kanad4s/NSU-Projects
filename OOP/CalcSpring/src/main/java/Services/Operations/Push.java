package Services.Operations;

import Services.CalculatorController.CalculatorStack;
import Services.InputChecker.InputChecker;

import java.util.ArrayList;

public class Push implements Operation{

    @Override
    public void calculation(CalculatorStack stack, ArrayList<String> listValues) throws Exception {
        if (listValues.size() == 1) {
            if (InputChecker.isDouble(listValues.get(0))) {
                stack.pushValue(listValues.get(0));
            } else if (stack.getMapDefineValues().containsKey(listValues.get(0))) {
                stack.pushValue(String.valueOf(stack.getDefineValue(listValues.get(0))));
            } else {
                throw new Exception("Defined value not found");
            }
        } else {
            throw new Exception("Invalid number of arguments PUSH");
        }
    }
}
