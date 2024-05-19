package ru.nsu.fit.tropin.Operations;

import org.springframework.stereotype.Component;
import ru.nsu.fit.tropin.CalculatorController.CalculatorStack;

import java.util.ArrayList;

@Component
public class Comment implements Operation{
    @Override
    public void calculation(CalculatorStack stack, ArrayList<String> listValues) throws Exception {
        for (String listValue : listValues) {
            System.out.println(listValue);
        }
    }
}
