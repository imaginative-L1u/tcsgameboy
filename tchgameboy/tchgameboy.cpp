//bg: 944*1506 /2   = 472*753
//screen: 806*621(53*41) /2  = 403*310 (26*20)
//start: 77,69 /2  = 38, 34
//exit:291*497 ---- 350*558
//rankings:368*462 ---- 428*520
//record:136*617 ---- 195*651

#include <stdio.h>
#include <cstdlib>
#include <easyx.h>
#include <fstream>
#include <string>
#include <utility>
#include <ctime>
#include <iostream>
#include <sstream>
#include <conio.h>
#include <cstdio>
#include <list>
#include <vector>
#include <algorithm>
#include <irrKlang.h>
#include "snake.h"

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#pragma comment(lib, "irrKlang.lib")
using namespace std;
using namespace irrklang;

IMAGE bkg, over, Snake, F, ranking, Snake2;
std::string name;
void rankings();
ISoundEngine* engine;


std::string welcome() {
    std::cout << "     _______..__   __.      ___       __  ___  _______ " << std::endl
            << "    /       ||  \\ |  |     /   \\     |  |/  / |   ____|" << std::endl
            << "   |   (----`|   \\|  |    /  ^  \\    |  '  /  |  |__   " << std::endl
            << "    \\   \\    |  . `  |   /  /_\\  \\   |    <   |   __|  " << std::endl
            << ".----)   |   |  |\\   |  /  _____  \\  |  .  \\  |  |____ " << std::endl
            << "|_______/    |__| \\__| /__/     \\__\\ |__|\\__\\ |_______|" << std::endl
            << std::endl;
    std::cout << "Enter your nickname:";
    std::string tmp;
    std::cin >> tmp;
    return tmp;
}

std::string get_time() {
    time_t raw;
    time(&raw);
    tm *res = localtime(&raw);
    std::stringstream ss;
    ss << res->tm_year + 1900 << "-" << res->tm_mon + 1 << "-" << res->tm_mday
        << "/" << res->tm_hour << ":" << res->tm_min << ":" << res->tm_sec;
    return ss.str();
}

//Record
void record(std::string name, int score) {
    std::ofstream writer("record.txt", std::ios::app);
    if (!writer.is_open())
        writer.open("record.txt");
    writer << name << " " << get_time() << " " << score << std::endl;
}

struct Rank
{
    string record_name;
    string record_time;
    int record_score = 0;
};

bool cmp(const Rank& a, const Rank& b) {
    return a.record_score > b.record_score;
}

int score = 0;

void create_window() {
    initgraph(472, 753);
}

void draw_bg() {
    putimage(0, 0, &bkg);
}

void draw_over(int j) {
    BeginBatchDraw();
    putimage(0, 0, &over);
    EndBatchDraw();
    TCHAR snake_score[50];
    sprintf_s(snake_score, _T("你的最终得分:%d"), score);
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    settextstyle(15, 10, _T("方正悠黑"));
    outtextxy(178, 270, snake_score);
    while (true)
    {


        if (j == 1) {
            engine->stopAllSounds();
            engine->play2D("resources/smb_gameover.wav", false);
        }
        j = 2;

        ExMessage msg;


        while (peekmessage(&msg, EX_MOUSE, PM_REMOVE))
        {
            msg = getmessage(EX_MOUSE);
            if (msg.message == WM_LBUTTONDOWN)
            {
                if (msg.x >= 291 && msg.x <= 350 && msg.y >= 497 && msg.y <= 558)
                {
                    exit(0);
                }
                if (msg.x >= 136 && msg.x <= 195 && msg.y >= 617 && msg.y <= 651)
                {
                    record(name, score);
                    engine->play2D("resources/smb_pause.wav", false);
                }
            }
            if (msg.message == WM_MOUSEMOVE)
            {
                if (msg.x >= 368 && msg.x <= 428 && msg.y >= 462 && msg.y <= 520)
                {
                    rankings();
                }
            }
        }

        Sleep(20);
    }
}

snake *s;

//变换x,y使其出现在403*310内
//range(x):[0,403]
//range(y):[0,310]
int convert_x(int x) { return x * 15 + 26; }
int convert_y(int y) { return y * 15 + 34; }

void snakeinit() {
    s = new snake(rand() % 25 + 1, rand() % 19 + 1);
}

void draw_snake() {
    auto p = s->head;
    while (p) {
        if (score <= 10)
        {
            putimage(convert_x(p->x), convert_y(p->y), &Snake);
            p = p->next;
        }
        else
        {
            putimage(convert_x(p->x), convert_y(p->y), &Snake2);
            p = p->next;
        }
    }
}

void keyboard() {
    while (_kbhit())
    {
        
        switch (_getch())
        {
        case 72:
        case 'w':
        case 'W':
            if (s->dy == 0)
                s->turn(0, -1);
            break;
        case 80:
        case 's':
        case 'S':
            if (s->dy == 0)
                s->turn(0, 1);
            break;
        case 75:
        case 'a':
        case 'A':
            if (s->dx == 0)
                s->turn(-1, 0);
            break;
        case 77:
        case 'd':
        case 'D':
            if (s->dx == 0)
                s->turn(1, 0);
            break;
        case ' ':
            system("pause");
            Sleep(100);
        default:
            break;
        }
    }
}

void snakemove() {
    keyboard();
    s->update(s->head, NULL);
}

std::list<std::pair<int, int>> food;

void create_food(int x, int y) {
    food.push_back(std::make_pair(x, y));
}

void show_food() {
    for (auto i : food) {
        //x:i.first, y:i.second
        putimage(convert_x(i.first), convert_y(i.second), &F);
    }
}

bool check_food(int x, int y) {
    for (auto i = food.begin(); i != food.end(); i++)
        if (i->first == x && i->second == y)
        {
            food.erase(i);
            return true;
        }
    return false;
}

void eatfood() {
    if (check_food(s->head->x, s->head->y))
    {
        engine->play2D("resources/smb_coin.wav", false);
        create_food(rand() % 25 + 1, rand() % 19 + 1);
        s->add_node();
        score++;
    }
}

void show_score() {
    TCHAR snake_score[50];
    sprintf_s(snake_score, _T("score:%d"), score);
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    settextstyle(13, 9, _T("方正悠黑"));
    outtextxy(38, 39, snake_score);
}

void rankings() {
    putimage(0, 0, &ranking);
    int n = 0, i = 0;
    vector<Rank> r;
    Rank temp;
    ifstream fp("record.txt");
    if (!fp)
    {
        printf("Unable to open the file");
    }
    while (fp >> temp.record_name >> temp.record_time >> temp.record_score)
    {
        r.push_back(temp);
    }
    fp.close();



    sort(r.begin(), r.end(), cmp);
    for (int i = 0; i < r.size(); i++)
    {
        cout << r[i].record_name << "  " << r[i].record_time << "  " << to_string(r[i].record_score) << endl;
    }


    if (r.size() >= 10)
     {
        for (int i = 0; i < 10; i++)
        {

            string output = r[i].record_name + "  " + r[i].record_time + "  " + to_string(r[i].record_score);
            char c[50];
            strcpy(c, output.c_str());
            setbkmode(TRANSPARENT);
            settextcolor(BLACK);
            settextstyle(13, 9, _T("方正悠黑"));
            outtextxy(120, 100 + 20 * i, c);
        }

    }
    if (i < 10)
    {
        for (int k = 0; k < i; k++)
        {
            string output = r[k].record_name + "  " + r[k].record_time + "  " + to_string(r[k].record_score);
            char c[50];
            strcpy_s(c, sizeof(c), output.c_str());
            setbkmode(TRANSPARENT);
            settextcolor(BLACK);
            settextstyle(13, 9, _T("方正悠黑"));
            outtextxy(120, 100 + 20 * k, c);

        }

    }

}

int i = 0;

void gameover() {

        if (s->head->x < 0 || s->head->x > 27 || s->head->y < 0 || s->head->y > 20) {
            i++;
            draw_over(i);
        }
        auto p = s->head->next;
        while (p) {
            if (p->x == s->head->x && p->y == s->head->y) {
                i++;
                draw_over(i);
                break;
            }
            p = p->next;
        }

}

int main()
{
    
    name = welcome();
    create_window();
    loadimage(&bkg, _T("resources/bkg.png"), 472, 753);
    loadimage(&over, _T("resources/over1.png"), 472, 753);
    loadimage(&Snake, _T("resources/block.png"), 15, 15);
    loadimage(&F, _T("resources/apple.png"), 15, 15, true);
    loadimage(&ranking, _T("resources/rankings1.png"), 472, 753);
    loadimage(&Snake2, _T("resources/snake2.png"), 15, 15);
    engine = createIrrKlangDevice();
    if (!engine)
        return 0; // error starting up the engine
    engine->play2D("resources/Sunny Jim.flac", true);
    snakeinit();
    create_food(rand() % 25 + 2, rand() % 19 + 2);
    while (true) {
        BeginBatchDraw();
        draw_bg();
        draw_snake(); 
        show_food();
        snakemove();
        eatfood();
        show_score();
        EndBatchDraw();
        gameover();
        Sleep(200);
    }

    getchar();
    return 0;
}

