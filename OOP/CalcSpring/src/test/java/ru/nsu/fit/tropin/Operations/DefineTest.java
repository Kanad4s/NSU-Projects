package ru.nsu.fit.tropin.Operations;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import ru.nsu.fit.tropin.CalculatorController.CalculatorStack;
import ru.nsu.fit.tropin.Exceptions.ArgumentsNumberException;

import java.util.ArrayList;

import static org.junit.jupiter.api.Assertions.*;

class DefineTest {
    private CalculatorStack calculatorStack;
    private Define define;
    private ArrayList<String> list;

    @BeforeEach
    void setUp() {
        calculatorStack = new CalculatorStack();
        define = new Define();
        list = new ArrayList<>();
    }

    @Test
    void define() throws Exception {
        String parametr = "a";
        String value = "10";
        list.add(parametr);
        list.add(value);
        define.calculation(calculatorStack, list);
        assertEquals(10, calculatorStack.getDefineValue(parametr));
    }

    @Test
    void defineWrongParametr() throws Exception {
        String parametr = "a";
        String value = "10";
        list.add(parametr + 1);
        list.add(value);
        define.calculation(calculatorStack, list);
        assertThrows(NullPointerException.class, () -> calculatorStack.getDefineValue(parametr));
    }

    @Test
    void calculationNotEnoughArgs() {
        String parametr = "a";
        list.add(parametr);
        assertThrows(ArgumentsNumberException.class, () -> define.calculation(calculatorStack, list));
    }
}