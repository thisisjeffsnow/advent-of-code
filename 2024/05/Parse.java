import java.io.*;
import java.util.*;

public class Parse {

    // Checking that a given 'sort' (group of pages) is valid to the rules graph
    private static boolean isValidSort(String[] sort, Map<String, List<String>> graph) {
        // Map each page to its position in the sort
        Map<String, Integer> placeholders = new HashMap<>();
        for (int i = 0; i < sort.length; i++) {
            placeholders.put(sort[i], i);
        }

        // Avoid null pointer here by only checking rules that exist in the current
        // sorting of pages.
        for (String node : sort) {
            // If we have no mention of rules with that page, we don't care.
            if (!graph.containsKey(node)) {
                continue;
            }

            for (String neighbor : graph.get(node)) {
                // If the rule applies to pages we're not even encountering, we also don't care.
                if (!placeholders.containsKey(neighbor)) {
                    continue;
                }

                // If the page in the sort violates a rule, fail.
                if (placeholders.get(node) > placeholders.get(neighbor)) {
                    return false;
                }
            }
        }

        return true;
    }

    private static List<String> fixBadSort(String[] badSort, Map<String, List<String>> graph) {
        // Filter out rules so that we only have a rules that apply to our sort
        // and make a new graph from only those rules.
        Map<String, List<String>> smallGraph = new HashMap<>();
        for (String node : badSort) {
            if (graph.containsKey(node)) {
                smallGraph.put(node, new ArrayList<>());
                for (String neighbor : graph.get(node)) {
                    if (Arrays.asList(badSort).contains(neighbor)) {
                        smallGraph.get(node).add(neighbor);
                    }
                }
            }
        }

        Set<String> processed = new HashSet<>();
        Stack<String> fixedSort = new Stack<>();

        // Recursively get the order of the pages from the rules relevant to the pages
        for (String node : smallGraph.keySet()) {
            if (!processed.contains(node)) {
                dfs(node, smallGraph, processed, fixedSort);
            }
        }

        return new ArrayList<>(fixedSort);
    }

    // helper for dfs fixing bad sorts
    private static void dfs(String node, Map<String, List<String>> graph, Set<String> processed, Stack<String> result) {
        processed.add(node);

        for (String neighbor : graph.getOrDefault(node, new ArrayList<>())) {
            if (!processed.contains(neighbor)) {
                dfs(neighbor, graph, processed, result);
            }
        }

        result.push(node);
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

        // Go through our sorts and determine validity.
        List<String> goodSorts = new ArrayList<>();
        List<String> goodMiddles = new ArrayList<>();
        int goodMiddlesSum = 0;

        List<String> badSorts = new ArrayList<>();
        List<String> badMiddles = new ArrayList<>();
        int badMiddlesSum = 0;

        for (String[] sort : sorts) {
            boolean isValid = isValidSort(sort, graph);

            if (isValid) {
                // Existing logic for valid sorts [Part 1]
                goodSorts.add(Arrays.toString(sort));
                String middle = sort[sort.length / 2];
                goodMiddles.add(middle);
                goodMiddlesSum += Integer.parseInt(middle);
            } else {
                // New logic for invalid sorts [Part 2]
                // Test to see output.
                // System.out.println("Invalid sort: " + Arrays.toString(sort));
                // fixBadSort(sort, graph); // Show rules graph from this sort.
                badSorts.add(Arrays.toString(sort));
                List<String> fixedSort = fixBadSort(sort, graph);
                String middle = fixedSort.get(fixedSort.size() / 2);
                badMiddles.add(middle);
                badMiddlesSum += Integer.parseInt(middle);
            }
        }

        // Stats
        // System.out.println("Middle elements of valid sorts: " + middles);
        System.out.println("[P1] Sum of good middles: " + goodMiddlesSum);
        System.out.println("[P2] Sum of bad middles: " + badMiddlesSum);
    }
}
