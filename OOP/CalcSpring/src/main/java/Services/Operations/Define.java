package Services.Operations;

import Services.CalculatorController.CalculatorStack;
import org.springframework.stereotype.Component;

import java.util.ArrayList;

//@Component
public class Define implements Operation {
    @Override
    public void calculation(CalculatorStack stack, ArrayList<String> listValues) throws Exception {
        if (listValues.size() == 2) {
            stack.addDefineValue(listValues.get(0), listValues.get(1));
        } else {
            throw new Exception("Invalid number of arguments DEFINE");
        }
    }
}
