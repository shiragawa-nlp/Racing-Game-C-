#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <clocale>

int SCREEN_WIDTH = 80;
int SCREEN_HEIGHT = 26;
int WIN_WIDTH = 60;

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

void initConsoleSize()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(console, &csbi))
    {
        int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        SCREEN_WIDTH = min(80, max(60, width));
        SCREEN_HEIGHT = min(26, max(24, height));
        WIN_WIDTH = SCREEN_WIDTH - 18;
        if (WIN_WIDTH < 50)
            WIN_WIDTH = 50;
    }
    else
    {
        SCREEN_WIDTH = 80;
        SCREEN_HEIGHT = 26;
        WIN_WIDTH = 60;
    }
}

int scoreX()
{
    // スコア表示を適切な位置に配置し、境界チェックを実施
    int x = WIN_WIDTH + 3;
    if (x > SCREEN_WIDTH - 20)
        x = SCREEN_WIDTH - 20;
    return x;
}

int enemyY[2];
int enemyX[2];
bool enemyFlag[2];

char car[4][4] = {
    {' ', '#', '#', ' '},
    {'#', '#', '#', '#'},
    {' ', '#', '#', ' '},
    {'#', '#', '#', '#'}
};

int carPos = WIN_WIDTH / 2;
int score = 0;

void gotoxy(int x, int y)
{
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size)
{
    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = size;
    SetConsoleCursorInfo(console, &lpCursor);
}

void drawBorder()
{
    for (int i = 0; i < SCREEN_HEIGHT; i++)
    {
        // 左枠を描画
        for (int j = 0; j < 17; j++)
        {
            gotoxy(j, i);
            cout << "|";
        }

        // 右内枠を描画
        for (int j = 0; j < 17; j++)
        {
            gotoxy(WIN_WIDTH - j, i);
            cout << "|";
        }

        // 右外枠を描画
        gotoxy(SCREEN_WIDTH - 1, i);
        cout << "|";
    }
}

void genEnemy(int ind)
{
    int minX = 17;
    int maxX = WIN_WIDTH - 21;  // 敵が境界内に留まるように調整
    if (maxX < minX)
        maxX = minX;
    enemyX[ind] = minX + rand() % (maxX - minX + 1);
}

void drawEnemy(int ind)
{
    if (enemyFlag[ind])
    {
        gotoxy(enemyX[ind], enemyY[ind]);     cout << "****";
        gotoxy(enemyX[ind], enemyY[ind] + 1); cout << " ** ";
        gotoxy(enemyX[ind], enemyY[ind] + 2); cout << "****";
        gotoxy(enemyX[ind], enemyY[ind] + 3); cout << " ** ";
    }
}

void eraseEnemy(int ind)
{
    if (enemyFlag[ind])
    {
        gotoxy(enemyX[ind], enemyY[ind]);     cout << "    ";
        gotoxy(enemyX[ind], enemyY[ind] + 1); cout << "    ";
        gotoxy(enemyX[ind], enemyY[ind] + 2); cout << "    ";
        gotoxy(enemyX[ind], enemyY[ind] + 3); cout << "    ";
    }
}

void resetEnemy(int ind)
{
    eraseEnemy(ind);
    enemyY[ind] = 1;
    genEnemy(ind);
}

void drawCar()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            gotoxy(carPos + j, 22 + i);
            cout << car[i][j];
        }
    }
}

void eraseCar()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            gotoxy(carPos + j, 22 + i);
            cout << " ";
        }
    }
}

int collision()
{
    for (int i = 0; i < 2; i++)
    {
        if (enemyFlag[i] && enemyY[i] + 4 >= 22)  // 23から22に調整
        {
            // 敵車（x：enemyX[i]からenemyX[i]+3）が
            // プレイヤー車（x：carPosからcarPos+3）と重なるかチェック
            if (enemyX[i] < carPos + 4 && enemyX[i] + 4 > carPos)
            {
                return 1;
            }
        }
    }
    return 0;
}

void gameover()
{
    system("cls");

    cout << "\n\t\t--------------------------";
    cout << "\n\t\t-------- ゲームオーバー -------";
    cout << "\n\t\t--------------------------\n";
    cout << "\n\t\t最終スコア: " << score;
    
    getch();
}

void updateScore()
{
    gotoxy(scoreX(), 5);
    cout << "スコア: " << score << "     ";  // 前の表示をクリアするため余分なスペースを追加
}

void instructions()
{
    system("cls");
    cout << "プロジェクトの概要\n";
    cout << "----------------\n";
    cout << "本プロジェクトでは、C++を用いて、Windowsのコンソール上で動作するカーゲームを作成しました。\n\n";
    cout << "操作方法：\n";
    cout << "- ←キー：左へ移動\n";
    cout << "- →キー：右へ移動\n";
    cout << "- ESCキー：ゲームを終了\n\n";
    cout << "プレイヤーは左右の矢印キーを使って車を移動させ、上から下へ移動してくる敵車を避けます。\n";
    cout << "敵車に衝突するとゲームオーバーとなり、最終スコアが表示されます。\n\n";
    
    getch();
}

void play()
{
    carPos = (WIN_WIDTH / 2) - 2;  // 幅4の車に対してより良いセンター配置
    score = 0;

    enemyFlag[0] = true;
    enemyFlag[1] = false;

    enemyY[0] = 1;
    enemyY[1] = 1;

    system("cls");

    drawBorder();
    updateScore();

    genEnemy(0);
    genEnemy(1);

    // 元の挙動に戻す: 表示無しでキー待ち（デモ用の可視プロンプトを削除）
    getch();

    while (true)
    {
        if (kbhit())
        {
            int ch = getch();

            if (ch == 0 || ch == 0xE0)
                ch = getch();

            if (ch == 0x4B) // 左矢印
            {
                if (carPos - 4 >= 17)
                    carPos -= 4;
            }

            if (ch == 0x4D) // 右矢印
            {
                if (carPos + 4 <= WIN_WIDTH - 21)  // genEnemy maxXに合わせて更新
                    carPos += 4;
            }

            if (ch == 27)
                break;
        }

        drawCar();
        drawEnemy(0);
        drawEnemy(1);

        if (collision())
        {
            gameover();
            return;
        }

        Sleep(50);

        eraseCar();
        eraseEnemy(0);
        eraseEnemy(1);

        if (enemyY[0] == 10 && !enemyFlag[1])
            enemyFlag[1] = true;

        if (enemyFlag[0]) enemyY[0]++;
        if (enemyFlag[1]) enemyY[1]++;

        if (enemyY[0] > SCREEN_HEIGHT - 4)
        {
            resetEnemy(0);
            score++;
            updateScore();
        }

        if (enemyY[1] > SCREEN_HEIGHT - 4)
        {
            resetEnemy(1);
            score++;
            updateScore();
        }
    }
}

int main()
{
    system("chcp 65001 > nul");
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, ".UTF-8");

    srand((unsigned)time(nullptr));
    setcursor(false, 20);
    initConsoleSize();

    while (true)
    {
        system("cls");

        cout << "\n\n";
        cout << "  --------------------------\n";
        cout << "  |      レーシングゲーム      |\n";
        cout << "  --------------------------\n\n";
        cout << "  1. ゲームを始める\n";
        cout << "  2. 操作方法\n";
        cout << "  3. 終了\n\n";
        cout << "  選択してください: ";

        char op = getche();

        if (op == '1')
            play();
        else if (op == '2')
            instructions();
        else if (op == '3')
            return 0;
    }
}