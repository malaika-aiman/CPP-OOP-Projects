#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <utility>
using namespace std;

const int rows = 10;
const int cols = 15;

vector<string> maze = {
    "###############",
    "#S....#.......#",
    "###.#.#.#####.#",
    "#...#...#.....#",
    "#.#####.#.###.#",
    "#.....#.#...#.#",
    "#.###.#.###.#.#",
    "#...#.#.....#.#",
    "###.#.#####...#",
    "#############E#"
};

void displayMaze(int playerRow, int playerCol) {
    cout << '\n';
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (r == playerRow && c == playerCol)
                cout << '@';
            else
                cout << maze[r][c];
        }
        cout << '\n';
    }
    cout << "Use W A S D to move and Q to quit.\n";
}

int shortestPath() {
    queue<pair<int, int>> q;
    vector<vector<int>> distance(rows, vector<int>(cols, -1));
    int startR = 1, startC = 1;
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    q.push({startR, startC});
    distance[startR][startC] = 0;

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        if (maze[current.first][current.second] == 'E')
            return distance[current.first][current.second];

        for (int i = 0; i < 4; i++) {
            int nr = current.first + dr[i];
            int nc = current.second + dc[i];

            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
                maze[nr][nc] != '#' && distance[nr][nc] == -1) {
                distance[nr][nc] = distance[current.first][current.second] + 1;
                q.push({nr, nc});
            }
        }
    }
    return -1;
}

int main() {
    int playerRow = 1, playerCol = 1;
    int moves = 0;
    char input;

    cout << "Maze Escape Game\n";
    cout << "Reach E without touching the walls.\n";

    while (true) {
        displayMaze(playerRow, playerCol);
        cout << "Your move: ";
        cin >> input;

        if (input == 'q' || input == 'Q') {
            cout << "Game ended. You made " << moves << " moves.\n";
            break;
        }

        int nextRow = playerRow;
        int nextCol = playerCol;

        if (input == 'w' || input == 'W') nextRow--;
        else if (input == 's' || input == 'S') nextRow++;
        else if (input == 'a' || input == 'A') nextCol--;
        else if (input == 'd' || input == 'D') nextCol++;
        else {
            cout << "Please use W, A, S, D or Q.\n";
            continue;
        }

        if (nextRow < 0 || nextRow >= rows || nextCol < 0 || nextCol >= cols ||
            maze[nextRow][nextCol] == '#') {
            cout << "That path is blocked.\n";
            continue;
        }

        playerRow = nextRow;
        playerCol = nextCol;
        moves++;

        if (maze[playerRow][playerCol] == 'E') {
            cout << "You escaped in " << moves << " moves!\n";
            int best = shortestPath();
            if (best != -1)
                cout << "Shortest possible route: " << best << " moves.\n";
            break;
        }
    }

    return 0;
}
