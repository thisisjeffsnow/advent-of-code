import java.io.*;
import java.util.*;

public class Parse {

    // Checks if a given 'sort' is valid based on the rules graph
    private static boolean isValidSort(String[] sort, Map<String, List<String>> graph) {
        // Map each page to its position in the sort
        Map<String, Integer> placeholders = new HashMap<>();
        for (int i = 0; i < sort.length; i++) {
            placeholders.put(sort[i], i);
        }

        // Validate the order of pages in the sort
        for (String page : sort) {
            if (!graph.containsKey(page))
                continue; // Skip pages with no rules

            for (String neighbor : graph.get(page)) {
                if (!placeholders.containsKey(neighbor))
                    continue; // Skip neighbors not in the sort
                if (placeholders.get(page) > placeholders.get(neighbor))
                    return false; // Rule violated
            }
        }
        return true;
    }

    // Generates a corrected order for an invalid sort using DFS
    private static List<String> fixBadSort(String[] badSort, Map<String, List<String>> graph) {
        Map<String, List<String>> smallGraph = new HashMap<>();

        // Build new graph containing only relevant rules
        for (String page : badSort) {
            if (graph.containsKey(page)) {
                smallGraph.put(page, new ArrayList<>());
                for (String neighbor : graph.get(page)) {
                    if (Arrays.asList(badSort).contains(neighbor)) {
                        smallGraph.get(page).add(neighbor);
                    }
                }
            }
        }

        Set<String> processed = new HashSet<>();
        Stack<String> fixedSort = new Stack<>();

        // Perform DFS to reorder a bad sort.
        for (String page : smallGraph.keySet()) {
            if (!processed.contains(page)) {
                dfs(page, smallGraph, processed, fixedSort);
            }
        }

        return fixedSort;
    }

    // Helper function for DFS
    private static void dfs(String page, Map<String, List<String>> graph, Set<String> processed, Stack<String> result) {
        processed.add(page);

        for (String neighbor : graph.getOrDefault(page, new ArrayList<>())) {
            if (!processed.contains(neighbor)) {
                dfs(neighbor, graph, processed, result);
            }
        }

        result.push(page);
    }

    public static void main(String[] args) {
        String inputFile = "input.txt";

        List<String[]> rules = new ArrayList<>();
        List<String[]> sorts = new ArrayList<>();

        // Read the input file and parse rules and sorts
        try (BufferedReader buffy = new BufferedReader(new FileReader(inputFile))) {
            String line;
            while ((line = buffy.readLine()) != null) {
                if (line.contains("|")) {
                    rules.add(line.trim().split("\\|")); // Parse rules
                } else if (line.contains(",")) {
                    sorts.add(line.trim().split(",")); // Parse sorts
                }
            }
        } catch (IOException e) {
            System.out.println("Error reading file: " + e.getMessage());
        }

        // Build the graph from rules
        Map<String, List<String>> graph = new HashMap<>();
        for (String[] rule : rules) {
            String left = rule[0];
            String right = rule[1];

            graph.putIfAbsent(left, new ArrayList<>());
            graph.get(left).add(right);
            graph.putIfAbsent(right, new ArrayList<>());
        }

        // Analyze sorts and calculate results
        List<String> goodSorts = new ArrayList<>();
        List<String> goodMiddles = new ArrayList<>();
        int goodMiddlesSum = 0;

        List<String> badSorts = new ArrayList<>();
        List<String> badMiddles = new ArrayList<>();
        int badMiddlesSum = 0;

        for (String[] sort : sorts) {
            boolean isValid = isValidSort(sort, graph);

            if (isValid) {
                goodSorts.add(Arrays.toString(sort));
                String middle = sort[sort.length / 2];
                goodMiddles.add(middle);
                goodMiddlesSum += Integer.parseInt(middle);
            } else {
                badSorts.add(Arrays.toString(sort));
                List<String> fixedSort = fixBadSort(sort, graph);
                String middle = fixedSort.get(fixedSort.size() / 2);
                badMiddles.add(middle);
                badMiddlesSum += Integer.parseInt(middle);
            }
        }

        // Output results
        System.out.println("[P1] Sum of good middles: " + goodMiddlesSum);
        System.out.println("[P2] Sum of bad middles: " + badMiddlesSum);
    }
}
