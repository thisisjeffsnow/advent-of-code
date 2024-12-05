import java.io.*;
import java.util.*;

public class Parse {
    public static void main(String[] args) {
        String inputFile = "input.txt";

        // "A|B" => ['A', 'B']
        List<String[]> rules = new ArrayList<>(); 

        // "A,B,C,D,E..." => ['A', 'B', 'C', ... ]
        List<String[]> sorts = new ArrayList<>(); 

        try (BufferedReader buffy = new BufferedReader(new FileReader(inputFile))) {
            String line;

            // Assuming:
            // That we'll always have rules before we hit a block of lists
            // Rules will always have "|" in them
            // Sorts will always have "," in them
            while ((line = buffy.readLine()) != null) {
                if (line.contains("|")) {
                    // Split A|B into [A, B] and add to rules
                    String[] rule = line.split("\\|");
                    rules.add(rule);
                } else if (line.contains(",")) {
                    // Split A,B,C... into [A, B, C, ...] and add to sorts
                    String[] sort = line.split(",");
                    sorts.add(sort);
                }
            }
        } catch (IOException e) {
            System.out.println("Error reading file: " + e.getMessage());
        }

        // Make sure we can see the rules and sorts and have stored them.
        System.out.println("Rules:");
        for (String[] rule : rules) {
            System.out.println(Arrays.toString(rule));
        }

        System.out.println("\nSorts:");
        for (String[] sort : sorts) {
            System.out.println(Arrays.toString(sort));
        }
    }
}
