package Exceptions;

public class OperationException extends IllegalArgumentException {
    public OperationException(String message) {
        super(message);
    }
}
