package ru.nsu.fit.tropin.Operations;

import ru.nsu.fit.tropin.CalculatorController.CalculatorStack;
import org.springframework.stereotype.Component;
import ru.nsu.fit.tropin.Exceptions.OperationException;

import java.util.ArrayList;
import java.util.EmptyStackException;

@Component
public class Pop implements Operation {

    @Override
    public void calculation(CalculatorStack stack, ArrayList<String> listValues) throws Exception {
        LOGGER.info ("Pop value");
        try {
            stack.popValue();
        } catch (EmptyStackException e) {
            throw new OperationException("Empty stack");
        }
    }
}
