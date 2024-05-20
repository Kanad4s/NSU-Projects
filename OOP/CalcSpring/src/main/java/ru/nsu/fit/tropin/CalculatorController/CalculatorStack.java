package ru.nsu.fit.tropin.CalculatorController;

import org.springframework.stereotype.Component;
import ru.nsu.fit.tropin.Services.InputChecker.InputChecker;
import org.springframework.expression.ExpressionException;

import java.util.EmptyStackException;
import java.util.HashMap;
import java.util.Map;
import java.util.Stack;

@Component
public class CalculatorStack {
    private Map<String, Double> _mapDefineValues = new HashMap<String, Double>();
    private Stack<Double> _stack = new Stack<Double>();

    public CalculatorStack() {}

    public void pushValue(String value) {
        if (InputChecker.isDouble(value)) {
            _stack.push(Double.parseDouble(value));
        } else {
            throw new ExpressionException("Wrong input value");
        }
    }

    public double popValue() {
        return _stack.pop();
    }

    public Double peekValue() {
        try {
            return _stack.peek();
        } catch (EmptyStackException e) {
            throw new EmptyStackException ();
        }
    }

    public void addDefineValue(String parametr, String value) {
        if (InputChecker.isParameter(parametr) && InputChecker.isDouble(value)) {
            _mapDefineValues.put(parametr, Double.parseDouble(value));
        }
    }

    public double popDefineValue(String parametr) {
        double returnVale = _mapDefineValues.get(parametr);
        _mapDefineValues.remove(parametr);
        return returnVale;
    }

    public double getDefineValue(String parametr) {
        return _mapDefineValues.get(parametr);
    }

    public Map<String, Double> getMapDefineValues() {
        return _mapDefineValues;
    }
}
