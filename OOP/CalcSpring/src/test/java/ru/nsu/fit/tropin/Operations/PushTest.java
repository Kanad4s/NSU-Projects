package ru.nsu.fit.tropin.Operations;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import ru.nsu.fit.tropin.CalculatorController.CalculatorStack;
import ru.nsu.fit.tropin.Exceptions.ArgumentsNumberException;
import ru.nsu.fit.tropin.Exceptions.FormatDouble;
import ru.nsu.fit.tropin.Exceptions.OperationException;

import java.util.ArrayList;

import static org.junit.jupiter.api.Assertions.*;

class PushTest {

    private CalculatorStack calculatorStack;
    private Push push;
    private ArrayList<String> list;

    @BeforeEach
    void setUp() {
        calculatorStack = new CalculatorStack();
        push = new Push();
        list = new ArrayList<>();
    }

    @Test
    void push() throws Exception {
        String parameter = "a";
        String value = "10";
        calculatorStack.addDefineValue(parameter, value);
        list.add(parameter);
        push.calculation(calculatorStack, list);
        assertEquals(10, calculatorStack.popValue());
    }

    @Test
    void defineWrongParameter() {
        String parameter = "a";
        String value = "10";
        calculatorStack.addDefineValue(parameter, value);
        list.add(parameter + 1);
        assertThrows(FormatDouble.class, () -> push.calculation(calculatorStack, list));
    }

    @Test
    void calculationNotEnoughArgs() {
        String parameter = "a";
        String value = "10";
        calculatorStack.addDefineValue(parameter, value);
        assertThrows(ArgumentsNumberException.class, () -> push.calculation(calculatorStack, list));
    }
}