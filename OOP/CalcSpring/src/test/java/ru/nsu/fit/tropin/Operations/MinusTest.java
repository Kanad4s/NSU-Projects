package ru.nsu.fit.tropin.Operations;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import ru.nsu.fit.tropin.CalculatorController.CalculatorStack;

import java.util.ArrayList;
import java.util.EmptyStackException;

import static org.junit.jupiter.api.Assertions.*;

class MinusTest {
    private CalculatorStack calculatorStack;
    private Minus minus;
    private ArrayList<String> list;

    @BeforeEach
    void setUp() {
        calculatorStack = new CalculatorStack();
        minus = new Minus();
        list = new ArrayList<>();
    }

    @Test
    void calculation() throws Exception {
        calculatorStack.pushValue("40");
        calculatorStack.pushValue("10");
        minus.calculation(calculatorStack, list);
        assertEquals(-30, calculatorStack.popValue());
    }

    @Test
    void calculationEmpty() {
        assertThrows(EmptyStackException.class, () -> minus.calculation(calculatorStack, list));
    }

    @Test
    void calculationNotEnoughArgs() {
        calculatorStack.pushValue("10");
        assertThrows(EmptyStackException.class, () -> minus.calculation(calculatorStack, list));
    }
}