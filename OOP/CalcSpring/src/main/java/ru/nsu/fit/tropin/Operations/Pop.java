package ru.nsu.fit.tropin.Operations;

import ru.nsu.fit.tropin.CalculatorController.CalculatorStack;
import org.springframework.stereotype.Component;

import java.util.ArrayList;

@Component
public class Pop implements Operation {

    @Override
    public void calculation(CalculatorStack stack, ArrayList<String> listValues) throws Exception {
        LOGGER.info ("Pop value");
        stack.popValue();
    }
}
