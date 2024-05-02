package com.example.calcspring;

import Lesson.LessonsConfiguration;
import Services.GreetingService;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import Services.CalculatorController.CalculatorController;

import java.io.FileNotFoundException;
import java.util.logging.Logger;

@SpringBootApplication
public class Main {
    private static final Logger logger = Logger.getLogger(Main.class.getName());

	public static void main(String[] args) throws FileNotFoundException {

        CalculatorController controller;
        if (args.length == 0) {
            controller = new CalculatorController();
        } else {
            controller = new CalculatorController(args[0]);
        }

		//SpringApplication.run(CalcSpringApplication.class, args);
        logger.info("Starting configuration...");
        ApplicationContext context = new AnnotationConfigApplicationContext(LessonsConfiguration.class);
        GreetingService greetingService = context.getBean(GreetingService.class);
        logger.info(greetingService.sayGreeting());
	}

}
