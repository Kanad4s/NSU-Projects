package Services.Operations;

import Services.CalculatorController.CalculatorStack;
import com.example.calcspring.Main;
import org.apache.log4j.Logger;

import java.util.ArrayList;

public interface Operation {
    public static final Logger LOGGER = Logger.getLogger(Operation.class.getName());
    public void calculation(CalculatorStack stack, ArrayList<String> listValues) throws Exception;
}
