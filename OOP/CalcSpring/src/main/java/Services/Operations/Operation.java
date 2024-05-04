package Services.Operations;

import Services.CalculatorController.CalculatorStack;

import java.util.ArrayList;

public interface Operation {
    public void calculation(CalculatorStack stack, ArrayList<String> listValues) throws Exception;
}
