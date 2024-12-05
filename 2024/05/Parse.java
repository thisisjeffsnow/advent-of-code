import java.io.*;
import java.util.*;

public class Parse {

    // Checking that a given 'sort' (group of pages) is valid to the rules graph
    private static boolean isValidSort(String[] sort, Map<String, List<String>> graph) {
        // map each page to its position in the sort
        Map<String, Integer> placeholders = new HashMap<>();
        for (int i = 0; i < sort.length; i++) {
            placeholders.put(sort[i], i);
        }

        // then check all the rules to make sure all rules are applied correctly
        // to the sort
        for (String node : graph.keySet()) {
            for (String neighbor : graph.get(node)) {
                // If the neighbor appears before the node, the sort is invalid
                if (placeholders.get(node) > placeholders.get(neighbor)) {
                    return false;
                }
            }
        }
        return true;
    }

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
                    String[] rule = line.trim().split("\\|");
                    rules.add(rule);
                } else if (line.contains(",")) {
                    // Split A,B,C... into [A, B, C, ...] and add to sorts
                    String[] sort = line.trim().split(",");
                    sorts.add(sort);
                }
            }
        } catch (IOException e) {
            System.out.println("Error reading file: " + e.getMessage());
        }

        // Make sure we can see the rules and sorts and have stored them.
        // System.out.println("Rules:");
        // for (String[] rule : rules) {
        // System.out.println(Arrays.toString(rule));
        // }

        // System.out.println("\nSorts:");
        // for (String[] sort : sorts) {
        // System.out.println(Arrays.toString(sort));
        // }

        // Build graph from our rules
        Map<String, List<String>> graph = new HashMap<>();

        for (String[] rule : rules) {
            // LEFT|RIGHT => [Left < Right]
            String left = rule[0];
            String right = rule[1];

            graph.putIfAbsent(left, new ArrayList<>());

            graph.get(left).add(right);

            graph.putIfAbsent(right, new ArrayList<>());
        }

        // // Test graph output:
        // System.out.println("\nGraph:");
        // for (String key : graph.keySet()) {
        // System.out.println(key + " node connects to " + graph.get(key));
        // }

        // Now, let's test a sort to see if it's valid.
        String[] firstSort = sorts.get(0);
        boolean isValid = isValidSort(firstSort, graph);
        System.out.println("\nFirst sort is " + (isValid ? "valid" : "invalid"));
        // :( NullPointerException here, need to figure out why, lunch break)

    }
}
