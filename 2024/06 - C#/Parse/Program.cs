using System;
using System.IO;

class Program
{
    static void Main(string[] args)
    {
        // Basic code first. Can we grab all the rows?
        string[] rows = File.ReadAllLines("input.txt");

        // Can we print all the rows?
        foreach (string row in rows)
        {
            Console.WriteLine(row);
        }
    }
}
