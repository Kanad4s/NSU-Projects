package ru.nsu.fit.tropin.Operations;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import ru.nsu.fit.tropin.CalculatorController.CalculatorStack;
import ru.nsu.fit.tropin.Exceptions.OperationException;

import java.util.ArrayList;
import java.util.EmptyStackException;

import static org.junit.jupiter.api.Assertions.*;

class PopTest {
    private CalculatorStack calculatorStack;
    private Pop pop;
    private ArrayList<String> list;

    @BeforeEach
    void setUp() {
        calculatorStack = new CalculatorStack();
        pop = new Pop();
        list = new ArrayList<>();
    }

    @Test
    void calculation() throws Exception {
        calculatorStack.pushValue("10");
        calculatorStack.pushValue("20");
        pop.calculation(calculatorStack, list);
        assertEquals(10, calculatorStack.popValue());
    }

    @Test
    void calculationBeforeEmpty() throws Exception {
        calculatorStack.pushValue("10");
        pop.calculation(calculatorStack, list);
        assertThrows(EmptyStackException.class, () -> calculatorStack.popValue());
    }

    @Test
    void calculationEmpty() {
        assertThrows(OperationException.class, () -> pop.calculation(calculatorStack, list));
    }
}