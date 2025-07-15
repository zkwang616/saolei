#include <graphics.h>
#include <conio.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>

using namespace std;

const int WIDTH = 10;  // 横向格子数
const int HEIGHT = 10; // 纵向格子数
const int CELL_SIZE = 30; // 每个格子的大小
const int NUM_MINES = 10; // 雷的数量
const int WINDOW_WIDTH = WIDTH * CELL_SIZE;
const int WINDOW_HEIGHT = HEIGHT * CELL_SIZE + 40; // 增加状态栏高度

struct Cell {
    bool isMine;     // 是否是雷
    bool isOpen;     // 是否被打开
    bool isFlagged;  // 是否标记为雷
    int adjacentMines; // 周围雷的数量
};

Cell grid[HEIGHT][WIDTH];  // 游戏格子
int openedCells = 0;  // 已打开的格子数
bool gameOverFlag = false; // 游戏结束标志
bool gameWinFlag = false;  // 游戏胜利标志

// 初始化游戏
void initGame() {
    srand(time(0));  // 设置随机种子

    // 初始化格子
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            grid[i][j].isMine = false;
            grid[i][j].isOpen = false;
            grid[i][j].isFlagged = false;
            grid[i][j].adjacentMines = 0;
        }
    }

    // 随机放置雷
    int mineCount = 0;
    while (mineCount < NUM_MINES) {
        int i = rand() % HEIGHT;
        int j = rand() % WIDTH;
        if (!grid[i][j].isMine) {
            grid[i][j].isMine = true;
            mineCount++;
        }
    }

    // 计算每个格子的相邻雷数
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (!grid[i][j].isMine) {
                int mineCount = 0;
                for (int x = -1; x <= 1; x++) {
                    for (int y = -1; y <= 1; y++) {
                        if (i + x >= 0 && i + x < HEIGHT && j + y >= 0 && j + y < WIDTH) {
                            if (grid[i + x][j + y].isMine) {
                                mineCount++;
                            }
                        }
                    }
                }
                grid[i][j].adjacentMines = mineCount;
            }
        }
    }

    // 重置游戏状态
    openedCells = 0;
    gameOverFlag = false;
    gameWinFlag = false;
}

// 绘制格子
void drawGrid() {
    // 绘制状态栏背景
    setfillcolor(DARKGRAY);
    fillrectangle(0, HEIGHT * CELL_SIZE, WINDOW_WIDTH, WINDOW_HEIGHT);

    // 显示剩余地雷数
    settextcolor(WHITE);
    settextstyle(16, 0, _T("宋体"));
    outtextxy(10, HEIGHT * CELL_SIZE + 10, _T("剩余地雷: "));
    int remainingMines = NUM_MINES;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (grid[i][j].isFlagged) {
                remainingMines--;
            }
        }
    }
    char mineStr[10];
    sprintf_s(mineStr, "%d", remainingMines);
    outtextxy(100, HEIGHT * CELL_SIZE + 10, mineStr);

    // 绘制游戏区域
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int x = j * CELL_SIZE;
            int y = i * CELL_SIZE;

            // 绘制格子边框
            setlinecolor(BLACK);
            rectangle(x, y, x + CELL_SIZE, y + CELL_SIZE);

            if (grid[i][j].isOpen) {
                // 已打开的格子
                setfillcolor(LIGHTGRAY);
                fillrectangle(x + 1, y + 1, x + CELL_SIZE - 1, y + CELL_SIZE - 1);

                if (grid[i][j].isMine) {
                    // 绘制地雷
                    setfillcolor(BLACK);
                    solidcircle(x + CELL_SIZE / 2, y + CELL_SIZE / 2, CELL_SIZE / 4);

                    // 如果是游戏结束，标红踩中的地雷
                    if (gameOverFlag) {
                        setfillcolor(RED);
                        solidcircle(x + CELL_SIZE / 2, y + CELL_SIZE / 2, CELL_SIZE / 6);
                    }
                }
                else {
                    // 显示周围雷的数量
                    if (grid[i][j].adjacentMines > 0) {
                        settextcolor(BLACK);

                        // 根据数字设置不同颜色
                        switch (grid[i][j].adjacentMines) {
                        case 1: settextcolor(BLUE); break;
                        case 2: settextcolor(GREEN); break;
                        case 3: settextcolor(RED); break;
                        case 4: settextcolor(MAGENTA); break;
                        case 5: settextcolor(BROWN); break;
                        case 6: settextcolor(CYAN); break;
                        case 7: settextcolor(BLACK); break;
                        case 8: settextcolor(DARKGRAY); break;
                        }

                        char numStr[2];
                        sprintf_s(numStr, "%d", grid[i][j].adjacentMines);
                        settextstyle(16, 0, _T("宋体"));
                        outtextxy(x + CELL_SIZE / 2 - 5, y + CELL_SIZE / 2 - 8, numStr);
                    }
                }
            }
            else {
                // 未打开的格子
                setfillcolor(LIGHTGRAY);
                fillrectangle(x + 1, y + 1, x + CELL_SIZE - 1, y + CELL_SIZE - 1);

                // 如果是标记过的格子
                if (grid[i][j].isFlagged) {
                    setfillcolor(RED);
                    solidrectangle(x + CELL_SIZE / 4, y + CELL_SIZE / 4, x + 3 * CELL_SIZE / 4, y + 3 * CELL_SIZE / 4);
                }
            }
        }
    }

    // 游戏结束时显示提示
    if (gameOverFlag) {
        setfillcolor(DARKGRAY);
        fillrectangle(WINDOW_WIDTH / 2 - 70, WINDOW_HEIGHT / 2 - 20, WINDOW_WIDTH / 2 + 70, WINDOW_HEIGHT / 2 + 20);
        settextcolor(RED);
        settextstyle(20, 0, _T("宋体"));
        outtextxy(WINDOW_WIDTH / 2 - 40, WINDOW_HEIGHT / 2 - 10, _T("游戏结束!"));
    }

    // 游戏胜利时显示提示
    if (gameWinFlag) {
        setfillcolor(DARKGRAY);
        fillrectangle(WINDOW_WIDTH / 2 - 70, WINDOW_HEIGHT / 2 - 20, WINDOW_WIDTH / 2 + 70, WINDOW_HEIGHT / 2 + 20);
        settextcolor(GREEN);
        settextstyle(20, 0, _T("宋体"));
        outtextxy(WINDOW_WIDTH / 2 - 40, WINDOW_HEIGHT / 2 - 10, _T("游戏胜利!"));
    }
}

// 打开格子
void openCell(int i, int j) {
    if (i < 0 || i >= HEIGHT || j < 0 || j >= WIDTH || grid[i][j].isOpen || grid[i][j].isFlagged) {
        return;
    }

    grid[i][j].isOpen = true;
    openedCells++;

    // 如果是空格子，自动打开周围的格子
    if (grid[i][j].adjacentMines == 0) {
        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                if (x == 0 && y == 0) continue; // 跳过自身
                openCell(i + x, j + y);
            }
        }
    }
}

// 检查游戏是否胜利
bool checkWin() {
    // 两种胜利条件：1. 所有非地雷格子都被打开 2. 所有地雷都被正确标记
    if (openedCells == HEIGHT * WIDTH - NUM_MINES) {
        return true;
    }

    int correctFlags = 0;
    int totalFlags = 0;

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (grid[i][j].isFlagged) {
                totalFlags++;
                if (grid[i][j].isMine) {
                    correctFlags++;
                }
            }
        }
    }

    return (correctFlags == NUM_MINES && totalFlags == NUM_MINES);
}

// 显示所有地雷
void showAllMines() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (grid[i][j].isMine) {
                grid[i][j].isOpen = true;
            }
        }
    }
}

// 游戏主循环
void gameLoop() {
    initGame();

    while (true) {
        drawGrid();

        // 检查游戏状态
        if (!gameOverFlag && !gameWinFlag) {
            if (checkWin()) {
                gameWinFlag = true;
                continue;
            }
        }

        // 监听鼠标点击事件
        MOUSEMSG msg = GetMouseMsg();
        switch (msg.uMsg) {
        case WM_LBUTTONDOWN: {
            int i = msg.y / CELL_SIZE;
            int j = msg.x / CELL_SIZE;

            // 确保点击在游戏区域内
            if (i >= 0 && i < HEIGHT && j >= 0 && j < WIDTH) {
                if (!gameOverFlag && !gameWinFlag) {
                    if (grid[i][j].isMine) {
                        // 踩到地雷，游戏结束
                        showAllMines();
                        gameOverFlag = true;
                    }
                    else {
                        openCell(i, j);
                    }
                }
                else {
                    // 游戏结束后点击重新开始
                    initGame();
                }
            }
            break;
        }
        case WM_RBUTTONDOWN: {
            int i = msg.y / CELL_SIZE;
            int j = msg.x / CELL_SIZE;

            // 确保点击在游戏区域内且游戏未结束
            if (i >= 0 && i < HEIGHT && j >= 0 && j < WIDTH && !gameOverFlag && !gameWinFlag) {
                if (!grid[i][j].isOpen) {
                    grid[i][j].isFlagged = !grid[i][j].isFlagged;
                }
            }
            break;
        }
        }
    }
}

int main() {
    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
    gameLoop();
    closegraph();
    return 0;
}