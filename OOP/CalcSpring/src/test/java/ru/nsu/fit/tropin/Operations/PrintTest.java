package ru.nsu.fit.tropin.Operations;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import ru.nsu.fit.tropin.CalculatorController.CalculatorStack;
import ru.nsu.fit.tropin.Exceptions.OperationException;

import java.util.ArrayList;

import static com.github.stefanbirkner.systemlambda.SystemLambda.tapSystemOut;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

class PrintTest {
    private CalculatorStack calculatorStack;
    private Print print;
    private ArrayList<String> list;

    @BeforeEach
    void setUp() {
        calculatorStack = new CalculatorStack();
        print = new Print();
        list = new ArrayList<>();
    }

    @Test
    void calculation() throws Exception{
        calculatorStack.pushValue("100000000000000000");
        String text = tapSystemOut(() -> print.calculation(calculatorStack, list));
        assertEquals("1.0E17", text.trim());
    }

    @Test
    void calculationEmpty() {
        assertThrows(OperationException.class, () -> print.calculation(calculatorStack, list));
    }
}