package ru.nsu.fit.tropin.Services.Parser;

import org.springframework.context.annotation.Primary;
import org.springframework.lang.NonNull;
import org.springframework.stereotype.Component;

import java.util.ArrayList;
import java.util.List;

@Component
@Primary
public class LineParserSimple implements LineParser {
    private static final char COMMENT_SYMBOL = '#';
    private ArrayList<String> _parametersList;
    private String _operation;

    @Override
    public void parse(String line) {
        if (line.isEmpty()) {
            return;
        }
        _parametersList = new ArrayList<>(List.of(line.split("\\s")));
        if (_parametersList.get(0).charAt(0) == COMMENT_SYMBOL) {
            _operation = "COMMENT";
        } else {
            _operation = _parametersList.get(0);
        }
        _parametersList.remove(0);
        for (int i = 0; i < _parametersList.size(); i++) {
            System.out.println(_parametersList.get(i));
        }
    }

    @Override
    public ArrayList<String> getParameters() {return _parametersList;}

    @Override
    public String getOperation() {return _operation;}
}
