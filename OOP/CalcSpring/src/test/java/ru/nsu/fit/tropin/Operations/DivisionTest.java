package ru.nsu.fit.tropin.Operations;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import ru.nsu.fit.tropin.CalculatorController.CalculatorStack;
import ru.nsu.fit.tropin.Exceptions.OperationException;

import java.util.ArrayList;
import java.util.EmptyStackException;

import static org.junit.jupiter.api.Assertions.*;

class DivisionTest {
    private CalculatorStack calculatorStack;
    private Division division;
    private ArrayList<String> list;

    @BeforeEach
    void setUp() {
        calculatorStack = new CalculatorStack();
        division = new Division();
        list = new ArrayList<>();
    }

    @Test
     void calculation() throws Exception {
        calculatorStack.pushValue("10");
        calculatorStack.pushValue("20");
        division.calculation(calculatorStack, list);
        assertEquals(2, calculatorStack.popValue());
    }

    @Test
    void divisionByZero() {
        calculatorStack.pushValue("0");
        calculatorStack.pushValue("20");
        assertThrows(OperationException.class, () -> division.calculation(calculatorStack, list));
    }

    @Test
    void calculationNotEnoughArgs() {
        calculatorStack.pushValue("10");
        assertThrows(EmptyStackException.class, () -> division.calculation(calculatorStack, list));
    }

}