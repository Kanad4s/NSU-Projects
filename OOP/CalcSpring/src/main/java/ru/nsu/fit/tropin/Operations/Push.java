package ru.nsu.fit.tropin.Operations;

import ru.nsu.fit.tropin.Exceptions.ArgumentsNumberException;
import ru.nsu.fit.tropin.Exceptions.FormatDouble;
import ru.nsu.fit.tropin.CalculatorController.CalculatorStack;
import ru.nsu.fit.tropin.Services.InputChecker.InputChecker;
import org.springframework.stereotype.Component;

import java.util.ArrayList;


@Component
public class Push implements Operation{

    @Override
    public void calculation (CalculatorStack stack, ArrayList<String> listValues) throws Exception {
        if (listValues.size() == 1) {
            LOGGER.debug ("Push value in stack");
            if (InputChecker.isDouble(listValues.get(0))) {
                stack.pushValue(listValues.get(0));
            } else if (stack.getMapDefineValues().containsKey(listValues.get(0))) {
                stack.pushValue(String.valueOf(stack.getDefineValue(listValues.get(0))));
            } else {
                throw new FormatDouble("Defined value not found");
            }
        } else {
            LOGGER.error ("Wrong number of PUSH arguments");
            throw new ArgumentsNumberException("Wrong number of arguments ");
        }
    }
}
