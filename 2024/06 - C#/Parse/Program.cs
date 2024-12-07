using System;
using System.IO;
using System.Collections.Generic;

class Program
{

    static bool SimulatePath(int startX, int startY, int startDir, char[][] map, int[][] dirs, int dirMod, int mapH, int mapW)
    {
        // Track the squares the guard visits with their direction at each square.
        // If we ever see the guard in the same spot with the same direction again -> it's a loop.
        HashSet<(int x, int y, int dir)> visited = new HashSet<(int x, int y, int dir)>();
        int currX = startX, currY = startY, currDir = startDir;

        Boolean done = false;

        while (!done)
        {
            // Have we been here before with this direction? If so, we're looping.
            if (visited.Contains((currX, currY, currDir))) {
                return true; // LOOP!
            }

            visited.Add((currX, currY, currDir));

            // Figure out where we're going next.
            int nextY = currY + dirs[currDir][0];
            int nextX = currX + dirs[currDir][1];

            // Check if we've wandered off the edge of the map. If so, no loop!
            if (nextY < 0 || nextY >= mapH || nextX < 0 || nextX >= mapW) {
                done = true;
                break;
            }

            // Check if we hit an obstacle. If so, pivot.
            if (map[nextY][nextX] == '#')  {
                currDir = (currDir + 1) % dirMod; // Turn right
            }
            else {
                currX = nextX;
                currY = nextY;
            } // Move forward
        }

        // If we break out of the loop (harhar), then it's not really a loop. The guard exited some other way.
        return false;
    }

    static void Main(string[] args)
    {
        // Grab file rows
        string[] rows = File.ReadAllLines("input.txt");

        int mapH = rows.Length;
        int mapW = rows[0].Length;

        // Move rows to a writable map, find all obstacles and find start
        // We're gonna need to track obstacles and the guard's starting position.
        // Going to map obstacles by row and column too for Part 2?
        // Dictionary<int, List<(int x, int y)>> obstaclesByRow = new Dictionary<int, List<(int x, int y)>>();
        // Dictionary<int, List<(int x, int y)>> obstaclesByCol = new Dictionary<int, List<(int x, int y)>>();

        List<(int x, int y)> obstacles = new List<(int x, int y)>();
        char[][] map = new char[mapH][];
        int startX = -1, startY = -1; // Start uninitialized for now.

        // Fill the map array and find the guard's starting position (^).
        for (int i = 0; i < mapH; i++)
            map[i] = new char[mapW];

        for (int y = 0; y < mapH; ++y)
        {
            for (int x = 0; x < mapW; ++x)
            {
                map[y][x] = rows[y][x];
                if (rows[y][x] == '^') {
                    // Found guard, keep x and y.
                    startX = x;
                    startY = y;
                }
                if (rows[y][x] == '#') {
                     obstacles.Add((x, y)); // obstacles
                }
            }
        }

        int[][] dirs = new int[4][];
        dirs[0] = new int[] { -1, 0 }; // north Y - 1, X
        dirs[1] = new int[] { 0, 1 }; // east Y, X + 1
        dirs[2] = new int[] { 1, 0 }; // south Y + 1, X
        dirs[3] = new int[] { 0, -1 }; // west Y, X - 1
        int dirMod = dirs.Length;
        int currDirIndex = 0;

        // P1: Count visited spots by guard in original path.
        int visited = 0;
        HashSet<(int x, int y)> visitedSpots = new HashSet<(int x, int y)>();
        int currX = startX;
        int currY = startY;

        Boolean done = false;
        // So, we move north (y - 1, x) until we hit # (or out of bounds = done), then rotate and go east (y, x + 1)
        // until we hit # (or out of bounds = done), then south, (y + 1, x), then west, (y, x - 1), over and over.
        // Until we go out of bounds. We'll keep track of each new square we visit by marking the cell with an X for visited.

        while (!done)
        {
            if (!visitedSpots.Contains((currX, currY)))
            {
                visited++;
                visitedSpots.Add((currX, currY));
            }

            // What is our next move?
            int nextY = currY + dirs[currDirIndex][0];
            int nextX = currX + dirs[currDirIndex][1];

            // If we escape the map, we're done. 
            if (nextY < 0 || nextY >= mapH || nextX < 0 || nextX >= mapW) {
                done = true;
                break;
            }

            // Check if we need to pivot
            if (map[nextY][nextX] == '#')
            {
                currDirIndex = (currDirIndex + 1) % dirMod;
            }
            else
            {
                Console.WriteLine("Currently at: " + currX + ", " + currY);
                currX = nextX;
                currY = nextY;
            }
        }

        Console.WriteLine("Spots visited: " + visited);

        // P2: Block off all visited spots one by one on original path and check for looping.
        int loopSpots = 0;

        foreach (var (x, y) in visitedSpots)
        {
            // We can't put an obstacle at the starting position.
            if (x == startX && y == startY) {
                continue;
            }

            // Make a fresh copy of the map to test this position.
            char[][] mapCopy = new char[mapH][];
            for (int i = 0; i < mapH; i++) {
                mapCopy[i] = (char[])map[i].Clone();
            }

            // Place the new obstacle.
            mapCopy[y][x] = '#';

            // Console.WriteLine($"Placing block at: ({x}, {y})");

            // Simulate the guard's patrol with the new obstacle.
            if (SimulatePath(startX, startY, 0, mapCopy, dirs, dirMod, mapH, mapW))
            {
                // Console.WriteLine($"Loop detected with: ({x}, {y})");
                loopSpots++;
            }
        }

        Console.WriteLine("Loop spots: " + loopSpots);
    }
}
