#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stack>
#include <thread>
#include <chrono>


using namespace std;

/*
# : 出口
* : 80 points
$ : 100 points
*/
vector<vector<char>> map; // 用vector來存動態陣列迷宮
vector<vector<int>> points; // 用vector來存動態陣列迷宮點數
pair<int, int> entrancePoint;
pair<int, int> exitPoint;

int energy;  // 紀錄能量

bool loadMap(const string& filename) {
    ifstream file("迷宮作業測試檔.txt");
    if (!file) {
        cout << "無法開啟檔案！" << endl;
        return false;
    }
    string line;
    while (getline(file, line)) {
        vector<char> rowMap;
        vector<int> rowPoints;
        for (char ch : line) {
            if (ch == '0' || ch == '1' || ch == '$' || ch == '*' || ch == '#') {
                rowMap.push_back(ch);
               
                if (ch == '$') {
                    rowPoints.push_back(100); // $ 為 100 點
                }
                else if (ch == '*') {
                    rowPoints.push_back(80);  // * 為 80 點
                }
                else if (ch == '#') {
                    rowPoints.push_back(0);   // 出口不加點數
                    exitPoint = { map.size(), rowMap.size() - 1 }; // 記錄出口位置
                } else {
                    rowPoints.push_back(0);   // 0 或 1 無點數
                }
            }
            



        }
        if (!rowMap.empty()) {
            map.push_back(rowMap);         // 加入迷宮地圖
            points.push_back(rowPoints);  // 將行加入迷宮
        }

        // 檢查四個邊，尋找入口
        for (int i = 0; i < map[0].size(); ++i) {
            if (map[0][i] == '0' || map[map.size() - 1][i] == '0') {
                entrancePoint = { map[0][i] == '0' ? 0 : map.size() - 1, i };
            }
        }
        for (int i = 0; i < map.size(); ++i) {
            if (map[i][0] == '0' || map[i][map[i].size() - 1] == '0') {
                entrancePoint = { i, map[i][0] == '0' ? 0 : map[i].size() - 1 };
            }
        }

    }
    file.close();
    return true;
}

void displayMap(const pair<int, int>& mousePosition) {
    system("cls");
    cout.flush();

    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[i].size(); ++j) {
            if (make_pair(i, j) == mousePosition) {
                cout << "@";  // 小老鼠的位置
            }
            else if (map[i][j] == '1') {
                cout << "█";   // 牆壁
            }
            else if (map[i][j] == '0') {
                cout << ' ';   // 路徑
            }
            else if (map[i][j] == '*') {
                cout << "▩";   // 80分的位置
            }
            else if (map[i][j] == '$') {
                cout << "◙";   // 100分的位置
            }
            else if (map[i][j] == '#') {
                cout << "◫";   // 出口
            }
            else if (map[i][j] == '.') {
                cout << ".";   // 小老鼠走過的路徑
            }
        }
        cout << endl;
    }

    // 顯示分數和座標
    cout << "當前能量: " << energy << endl;
    cout << "當前座標: (" << mousePosition.first << ", " << mousePosition.second << ")" << endl;

    this_thread::sleep_for(chrono::milliseconds(100));  // 暫停0.1秒以模擬小老鼠的移動
}

// 小老鼠探索迷宮
bool findExit() {
    stack<pair<int, int>> pathStack;
    vector<vector<bool>> visited(map.size(), vector<bool>(map[0].size(), false));

    pathStack.push(entrancePoint);
    visited[entrancePoint.first][entrancePoint.second] = true;

    while (!pathStack.empty()) {
        pair<int, int> current = pathStack.top();
        int x = current.first;
        int y = current.second;

        if (current == exitPoint) {
            cout << "找到了出口！" << endl;
            return true;
        }

        if (energy == 0) {
            cout << "沒有能量了" << endl;
            return false;
            
        } else {
            energy -= 1;
        }


        map[x][y] = '.';  // 標記小老鼠走過的路徑
        displayMap(current);

        // 嘗試四個方向
        bool moved = false;
        const vector<pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
        for (auto dir : directions) {
            int newX = x + dir.first;
            int newY = y + dir.second;

            if (newX >= 0 && newX < map.size() && newY >= 0 && newY < map[0].size() && map[newX][newY] != '1' && !visited[newX][newY]) {

                visited[newX][newY] = true;
                pathStack.push({ newX, newY });

                // 根據小老鼠所走過的位置計算分數
                if (map[newX][newY] == '*') {
                    energy += 80;  // 走過 * 加 80 能量
                }
                else if (map[newX][newY] == '$') {
                    energy += 100; // 走過 $ 加 100 能量
                }

                moved = true;
                break;
            }
        }

        if (!moved) {
            pathStack.pop();
            displayMap(pathStack.empty() ? current : pathStack.top());  // 顯示回退時的位置
        }
    }

    cout << "迷宮無解！" << endl;
    return false;
}

int main() {
    cout << "輸入能量";
    cin >> energy;
    if (loadMap("maze.txt")) {
        cout << "迷宮地圖已讀取：" << endl;

        if (findExit()) {
            cout << "小老鼠成功找到了出口！" << endl;
        } else {
            cout << "沒能量失敗了：" << endl;
        }

    }
    else {
        cout << "讀取迷宮地圖失敗" << endl;
        cout << "按下 ENTER 結束程式..." << endl;

        // 等待使用者按下 ENTER
        cin.get();
    }
    return 0;
}