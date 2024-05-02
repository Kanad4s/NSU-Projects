package org.example;
import Services.GreetingService;
import lessons.LessonsConfiguration;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import org.springframework.context.annotation.Configuration;

import java.util.logging.LogManager;
import java.util.logging.Logger;

public class Main {
    //private static final Logger logger = LogManager.getLogManager().getLogger(String.valueOf(Main.class));

    public static void main(String[] args) {

        //l/ogger.info("Starting configuration...");

        ApplicationContext context = new AnnotationConfigApplicationContext(LessonsConfiguration.class);
        GreetingService greetingService = context.getBean(GreetingService.class);
        //logger.info(greetingService.sayGreeting());
    }
}