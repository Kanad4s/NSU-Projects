package ru.nsu.fit.tropin.Operations;

import ru.nsu.fit.tropin.CalculatorController.CalculatorStack;
import org.springframework.stereotype.Component;

import java.util.ArrayList;
import java.util.EmptyStackException;

@Component
public class Print implements Operation{
    @Override
    public void calculation (CalculatorStack stack, ArrayList<String> listValues) throws Exception {
        try {
            LOGGER.debug ("Print value");
            System.out.println(stack.peekValue());
        } catch (EmptyStackException e) {
            LOGGER.error ("Failed pop element " + this.getClass ());
            throw new EmptyStackException();
        }
    }
}
