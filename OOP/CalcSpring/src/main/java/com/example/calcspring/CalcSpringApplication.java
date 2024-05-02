package com.example.calcspring;

import Lesson.LessonsConfiguration;
import Services.GreetingService;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

import java.util.logging.Logger;

@SpringBootApplication
public class CalcSpringApplication {
    private static final Logger logger = Logger.getLogger(CalcSpringApplication.class.getName());

	public static void main(String[] args) {
		//SpringApplication.run(CalcSpringApplication.class, args);
        logger.info("Starting configuration...");
        ApplicationContext context = new AnnotationConfigApplicationContext(LessonsConfiguration.class);
        GreetingService greetingService = context.getBean(GreetingService.class);
        logger.info(greetingService.sayGreeting());
	}

}
