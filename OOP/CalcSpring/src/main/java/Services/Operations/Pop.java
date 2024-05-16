package Services.Operations;

import Services.CalculatorController.CalculatorStack;

import java.util.ArrayList;

@CommandAnnotation
public class Pop implements Operation {

    @Override
    public void calculation(CalculatorStack stack, ArrayList<String> listValues) throws Exception {
        LOGGER.info ("Pop value");
        stack.popValue();
    }
}
