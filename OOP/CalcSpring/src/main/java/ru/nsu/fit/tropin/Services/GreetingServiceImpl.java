package ru.nsu.fit.tropin.Services;

public class GreetingServiceImpl implements GreetingService {
    @Override
    public String sayGreeting() {
        return "Greeting, user!";
    }
}
