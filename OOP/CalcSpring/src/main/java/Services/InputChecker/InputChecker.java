package Services.InputChecker;

public class InputChecker {
    public static boolean isDouble(String value) {
        try {
            Double.parseDouble(value);
            return true;
        } catch (NumberFormatException e){
            return false;
        }
    }

    public static boolean isParameter(String value) {
        if (!Character.isLetter(value.charAt(0))) {
            return false;
        }
        for (int i = 1; i < value.length(); i++) {
            if (!Character.isDigit(value.charAt(i)) || !Character.isLetter(value.charAt(i))) {
                return false;
            }
        }
        return true;
    }
}
