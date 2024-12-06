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
        for (int y = 0; y < mapH; ++y)
        {
            for (int x = 0; x < mapW; ++x)
            {
                if (map[y][x] == '^')
                {
                    currX = x;
                    currY = y;
                }
            }
        }

        if (currX != -1 && currY != -1)
        {
            Console.WriteLine($"Start found at: ({currX}, {currY})");
        }
        else
        {
            Console.WriteLine("Start symbol '^' not found.");
        }


    }
}
