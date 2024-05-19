package ru.nsu.fit.tropin.Lesson;

import ru.nsu.fit.tropin.Services.GreetingService;
import ru.nsu.fit.tropin.Services.GreetingServiceImpl;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class LessonsConfiguration {
    @Bean
    GreetingService greetingService() {
        return new GreetingServiceImpl();
    }

}
