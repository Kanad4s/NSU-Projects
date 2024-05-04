package Services.Parser;

import org.springframework.stereotype.Component;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

public class LineParserSimple implements LineParser {
    private static final char COMMENT_SYMBOL = '#';
    private ArrayList<String> _parametersList;
    private String _operation;

    @Override
    public void parse(String line) {
        _parametersList = new ArrayList<>(List.of(line.split("\\s")));
        if (_parametersList.get(0).charAt(0) == COMMENT_SYMBOL) {
            _operation = "COMMENT";
        } else {
            _operation = _parametersList.get(0);
        }
        _parametersList.remove(0);
    }

    @Override
    public ArrayList<String> getParameters() {return _parametersList;}

    @Override
    public String getOperation() {return _operation;}
}
