#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Count words in a given line
int wordCountHelper(const string &line, const string &word)
{
    int count = 0;
    size_t pos = line.find(word);
    while (pos != string::npos)
    {
        count++;
        pos = line.find(word, pos + 3);
        // We can skip 3 chars out of XMAS each time because it can't overlap any sooner than that.
    }
    return count;
}

// Rows
int rowSearch(const vector<vector<char>> &letters, const string &word)
{
    string drow = string(word.rbegin(), word.rend());

    int count = 0;
    for (const auto &row : letters)
    {
        string line(row.begin(), row.end());
        count += wordCountHelper(line, word);
        count += wordCountHelper(line, drow);
    }
    return count;
}

// Cols
int colSearch(const vector<vector<char>> &letters, const string &word)
{
    string drow = string(word.rbegin(), word.rend());

    int count = 0;
    int rows = letters.size();
    int cols = letters[0].size();

    for (int col = 0; col < cols; col++)
    {
        string line;
        for (int row = 0; row < rows; row++)
        {
            line += letters[row][col];
        }
        count += wordCountHelper(line, word);
        count += wordCountHelper(line, drow);
    }
    return count;
}

// Diags
int diagSearch(const vector<vector<char>> &letters, const string &word)
{
    string drow = string(word.rbegin(), word.rend());

    int count = 0;
    int rows = letters.size();
    int cols = letters[0].size();

    // Top Left
    for (int start = 0; start < rows + cols - 1; ++start)
    {
        string line;
        for (int x = max(0, start - cols + 1), y = max(0, cols - 1 - start); x < rows && y < cols; ++x, ++y)
        {
            line += letters[x][y];
        }
        count += wordCountHelper(line, word);
        count += wordCountHelper(line, drow);
    }

    // Top Right
    for (int start = 0; start < rows + cols - 1; ++start)
    {
        string line;
        for (int x = max(0, start - cols + 1), y = min(cols - 1, start); x < rows && y >= 0; ++x, y--)
        {
            line += letters[x][y];
        }
        count += wordCountHelper(line, word);
        count += wordCountHelper(line, drow);
    }

    return count;
}

// Find X-MAS Special Pattern
int xmasCrossSearch(const vector<vector<char>> &letters)
{
    int count = 0;
    int rows = letters.size();
    int cols = letters[0].size();

    // Can skip edges as A is in the center
    for (int i = 1; i < rows - 1; ++i)
    {
        for (int j = 1; j < cols - 1; ++j)
        {
            if (letters[i][j] == 'A')
            {
                // Collect diagonals
                char topLeft = letters[i - 1][j - 1];
                char topRight = letters[i - 1][j + 1];
                char bottomLeft = letters[i + 1][j - 1];
                char bottomRight = letters[i + 1][j + 1];

                // Count S's and M's
                int sCount = (topLeft == 'S') + (topRight == 'S') + (bottomLeft == 'S') + (bottomRight == 'S');
                int mCount = (topLeft == 'M') + (topRight == 'M') + (bottomLeft == 'M') + (bottomRight == 'M');

                // Must have exactly 2 S's and 2 M's
                if (sCount == 2 && mCount == 2)
                {
                    // Check alignment: S's must share the same row or column
                    if (!((topLeft == 'S' && bottomRight == 'S') || (topRight == 'S' && bottomLeft == 'S')))
                    {
                        count++;
                    }
                }
            }
        }
    }

    return count;
}

int main()
{
    ifstream inputFile("input.txt");
    if (!inputFile.is_open())
    {
        cerr << "input.txt not found in current directory" << endl;
        return 1;
    }

    vector<vector<char>> letters;
    string line;
    while (getline(inputFile, line))
    {
        vector<char> row(line.begin(), line.end());
        letters.push_back(row);
    }
    inputFile.close();

    if (letters.empty())
    {
        cerr << "The input file is empty or not properly formatted." << endl;
        return 1;
    }

    string word = "XMAS";

    // Count matches in all directions, just in case they ask for specifics in Part 2
    int rowMatches = rowSearch(letters, word);
    int colMatches = colSearch(letters, word);
    int diagMatches = diagSearch(letters, word);
    int xmasCrossMatches = xmasCrossSearch(letters);

    // Output results
    cout << "Row Matches: " << rowMatches << endl;
    cout << "Col Matches: " << colMatches << endl;
    cout << "Diag Matches: " << diagMatches << endl;
    cout << "Part 1 Matches: " << (rowMatches + colMatches + diagMatches) << endl
         << endl;
    cout << "X-Mas Cross Matches: " << xmasCrossMatches << endl;
    return 0;
}
