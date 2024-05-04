package Configurations;

import Services.CalculatorController.CalculatorController;
import Services.CalculatorController.CalculatorControllerMain;
import Services.Operations.Operation;
import Services.Operations.Push;
import Services.Parser.LineParser;
import Services.Parser.LineParserSimple;
import jdk.jfr.Name;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Scope;

@Configuration
public class Configurations {

    @Bean
    CalculatorController calculatorController() {
        return new CalculatorControllerMain();
    }

    @Bean
    LineParser lineParser() {
        return new LineParserSimple();
    }

    @Bean
    Operation operation() {
        return new Push();
    }
}
