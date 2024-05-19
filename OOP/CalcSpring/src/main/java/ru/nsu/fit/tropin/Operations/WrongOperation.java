package ru.nsu.fit.tropin.Operations;

import org.springframework.stereotype.Component;
import ru.nsu.fit.tropin.CalculatorController.CalculatorStack;

import java.util.ArrayList;

@Component
public class WrongOperation implements Operation{
    @Override
    public void calculation(CalculatorStack stack, ArrayList<String> listValues) throws Exception {
        LOGGER.info("Wrong operation");
        System.err.println("Wrong operation");
//        throw new Exception("Wrong operation");
    }
}
