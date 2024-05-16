package Services.Operations;

import Exceptions.ArgumentsNumberException;
import Services.CalculatorController.CalculatorStack;

import java.util.ArrayList;

//@Component
@CommandAnnotation
public class Define implements Operation {
    @Override
    public void calculation(CalculatorStack stack, ArrayList<String> listValues) throws Exception {
        if (listValues.size() == 2) {
            LOGGER.debug("DEFINE " + listValues.get(0) + " as " + listValues.get(1));
            stack.addDefineValue(listValues.get(0), listValues.get(1));
        } else {
            LOGGER.debug("Invalid number of DEFINE arguments ");
            throw new ArgumentsNumberException("Invalid number of arguments");
        }
    }
}
