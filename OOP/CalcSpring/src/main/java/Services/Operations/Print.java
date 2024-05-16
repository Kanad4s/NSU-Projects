package Services.Operations;

import Services.CalculatorController.CalculatorStack;

import java.util.ArrayList;
import java.util.EmptyStackException;

@CommandAnnotation
public class Print implements Operation{
    @Override
    public void calculation (CalculatorStack stack, ArrayList<String> listValues) throws Exception {
        try {
            LOGGER.debug ("Try to print value");
            System.out.println(stack.peekValue());
        } catch (EmptyStackException e) {
            LOGGER.error ("Failed pop element " + this.getClass ());
            throw new EmptyStackException();
        }
    }
}
