using System;
using System.IO;
using System.Runtime.CompilerServices;

class Program
{
    static void Main(string[] args)
    {
        // Basic code first. Can we grab all the rows?
        string[] map = File.ReadAllLines("input.txt");

        // Can we print all the rows?
        // foreach (string row in rows)
        // {
        //     Console.WriteLine(row);
        // }

        // Alright, make the map a 2D array for all the characters
        // We see the rows are all the same length (we'll assume that's the case)
        // int mapH = rows.Length;
        // int mapW = rows[0].Length;
        // char[,] map = new char[mapH, mapW];

        // for(int y = 0; y < mapH; ++y) {
        //     for(int x = 0; x < mapW; ++x) {
        //         map[x, y] = rows[]
        //     }
        // }

        int mapH = map.Length;
        int mapW = map[0].Length;

        // Okay, good, find all obstacles and find start
        int currX = -1;
        int currY = -1;

        // It's already a 2D array. Print using x and y to test:
        // Find the start?
        // List to track obstacle positions
        List<(int x, int y)> obstacles = new List<(int x, int y)>();

        // Iterate over the map
        for (int y = 0; y < mapH; ++y)
        {
            for (int x = 0; x < mapW; ++x)
            {
                if (map[y][x] == '^') // Find start
                {
                    currX = x;
                    currY = y;
                }
                else if (map[y][x] == '#') // Find obstacles
                {
                    obstacles.Add((x, y));
                }
            }
        }

        // if (currX != -1 && currY != -1)
        // {
        //     Console.WriteLine($"Start found at: ({currX}, {currY})");
        // }
        // else
        // {
        //     Console.WriteLine("Start symbol '^' not found.");
        // }

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
            ++visited;

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

            // just loops forever? lol what
            Console.WriteLine("Currently at: " + currX + ", " + currY + "\n");
            currX = nextX;
            currY = nextY;

        }

        Console.WriteLine("Spots visited: " + visited);
    }

}
