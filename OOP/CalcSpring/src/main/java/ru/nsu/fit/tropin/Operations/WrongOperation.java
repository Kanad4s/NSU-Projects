package ru.nsu.fit.tropin.Operations;

import org.springframework.stereotype.Component;
import ru.nsu.fit.tropin.CalculatorController.CalculatorStack;

import java.util.ArrayList;

@Component
public class WrongOperation implements Operation{
    @Override
    public void calculation(CalculatorStack stack, ArrayList<String> listValues) throws Exception {
        System.out.println("Wrong operation");
    }
}
