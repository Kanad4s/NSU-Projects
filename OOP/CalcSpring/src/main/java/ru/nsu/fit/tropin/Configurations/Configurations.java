package ru.nsu.fit.tropin.Configurations;

import org.springframework.context.annotation.ComponentScan;
import ru.nsu.fit.tropin.CalculatorController.CalculatorController;
import ru.nsu.fit.tropin.CalculatorController.CalculatorControllerMain;
import ru.nsu.fit.tropin.Services.Parser.LineParser;
import ru.nsu.fit.tropin.Services.Parser.LineParserSimple;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
@ComponentScan(basePackages = "ru.nsu.fit.tropin")
public class Configurations {}
