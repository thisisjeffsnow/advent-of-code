using System;
using System.IO;
using System.Runtime.CompilerServices;

class Program
{
    static void Main(string[] args)
    {
        // Grab file rows
        string[] rows = File.ReadAllLines("input.txt");

        int mapH = rows.Length;
        int mapW = rows[0].Length;

        // Move rows to a writable map, find all obstacles and find start
        List<(int x, int y)> obstacles = new List<(int x, int y)>();

        // Going to map obstacles by row and column too for Part 2.
        Dictionary<int, List<(int x, int y)>> obstaclesByRow = new Dictionary<int, List<(int x, int y)>>();
        Dictionary<int, List<(int x, int y)>> obstaclesByCol = new Dictionary<int, List<(int x, int y)>>();

        int currX = -1;
        int currY = -1;

        char[][] map = new char[mapH][];
        for (int i = 0; i < mapH; i++)
        {
            map[i] = new char[mapW];
        }

        for (int y = 0; y < mapH; ++y)
        {
            for (int x = 0; x < mapW; ++x)
            {
                if (rows[y][x] == '^') // Find start
                {
                    currX = x;
                    currY = y;
                }
                else if (rows[y][x] == '#') // Find obstacles
                {
                    obstacles.Add((x, y));
                    if (!obstaclesByRow.ContainsKey(y))
                    {
                        obstaclesByRow[y] = new List<(int x, int y)>();
                    }
                    obstaclesByRow[y].Add((x, y));

                    if (!obstaclesByCol.ContainsKey(x))
                    {
                        obstaclesByCol[x] = new List<(int x, int y)>();
                    }
                    obstaclesByCol[x].Add((x, y));
                }
                map[y][x] = rows[y][x];
            }
        }


        int[][] dirs = new int[4][];
        dirs[0] = new int[] { -1, 0 }; // north Y - 1, X
        dirs[1] = new int[] { 0, 1 }; // east Y, X + 1
        dirs[2] = new int[] { 1, 0 }; // south Y + 1, X
        dirs[3] = new int[] { 0, -1 }; // west Y, X - 1
        int dirMod = dirs.Length;
        int currDirIndex = 0;

        Boolean done = false;
        // So, we move north (y - 1, x) until we hit # (or out of bounds = done), then rotate and go east (y, x + 1)
        // until we hit # (or out of bounds = done), then south, (y + 1, x), then west, (y, x - 1), over and over.
        // Until we go out of bounds. We'll keep track of each new square we visit by marking the cell with an X for visited.

        int visited = 0;
        while (!done)
        {
            if (map[currY][currX] != 'X')
            {
                ++visited;
                map[currY][currX] = 'X';
            }

            // What is our next move?
            int nextY = currY + dirs[currDirIndex][0];
            int nextX = currX + dirs[currDirIndex][1];

            // If we escape the map, we're done. 
            if (nextY < 0 || nextY >= mapH || nextX < 0 || nextX >= mapW)
            {
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
                Console.WriteLine("Currently at: " + currX + ", " + currY + "\n");
                currX = nextX;
                currY = nextY;
            }
        }

        Console.WriteLine("Spots visited: " + visited);

        // Check we can print obstacles by row and col:
        Console.WriteLine("Rows:");
        foreach (var row in obstaclesByRow)
        {
            Console.WriteLine($"Row {row.Key}: {string.Join(", ", row.Value)}");
        }

        // Print encountered obstacles by column
        Console.WriteLine("Cols:");
        foreach (var col in obstaclesByCol)
        {
            Console.WriteLine($"Column {col.Key}: {string.Join(", ", col.Value)}");
        }
    }

}
