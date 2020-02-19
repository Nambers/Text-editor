/*
*MAIL:1930893235@qq.com
*AUTHOR:n-N-n
*/
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <io.h>
#include <fstream>
#include <string>

#define MAX 1023
using namespace std;

unsigned char buffer[1024][1024];
void move_l(int h,int pos) {
    int a = 1;
    while (1) {
        buffer[h][pos + a-1]= buffer[h][pos + a ];
        if (buffer[h][pos + a - 1] == '\0')  break; 
        a++;
    }
}
void move_r(int h, int pos,char ch) {
    int a = 1;
    char temp,temp1;
    temp = buffer[h][pos + a];
    buffer[h][pos + a] = buffer[h][pos + a - 1];
    buffer[h][pos] = ch;
    while (1) {
        a++;
        temp1 = buffer[h][pos + a];
        buffer[h][pos + a] = temp;
        temp = temp1;
        if (temp == '\0')  break;
    }
}
void gotoxy(int x, int y) {
    COORD pos = { x,y };
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄
    SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置
}
int getlong(int h){
    int a = 0;
    while (1) {
        if (buffer[h][a] == '\0')return a;
        a++;
    }
}
void pout(int h, int pos=-1,bool all=false) {
    gotoxy(0, h);
    printf("%c[2K", 27);
    gotoxy(0, h);
    int a = 0;
    while (1) {
        if (buffer[h][a] == '\0') break;
        cout.put(buffer[h][a]);
        a++;
    }
    if (pos != -1) gotoxy(pos,h);
    if (all) {
        if (buffer[h + 1][0] != '~')pout(h+1,-1,true);
    }
}
void move_d(int h) {
    int a = 1;
    char temp[1024],temp1[1024];
    while (1) {
        int b = 0;
        while (1) {
            temp1[b] = buffer[h + a][b];
            if (a == 1){
                buffer[h + a][b] = buffer[h + a - 1][b];
                buffer[h + a - 1][b] = '\0';
            }
            else {
                buffer[h + a][b] = temp[b];
            }
            temp[b] = temp1[b];
            if (buffer[a][b + 1] == '\0')break;
            b++;
        }
        if (buffer[a + 1][0] == '~')break;
        a++;
    }
}
int main(int argc, char* argv[]) {
    if (argc == 1) {
        cout << "缺少参数";
        return 1;
   }
    if (argc > 2) {
        cout << "参数过多";
        return 1;
    }
    for (int i = 0; i < 1024; i++) {
        buffer[i][0] = '~';
    }
    int h = 0;
    int pos = 0;
    if (_access(argv[1], 00)==0) {
        //0 yes -1 no
        if (_access(argv[1], 06) == -1) {
            cout << "该文件无法读写";
            exit(EXIT_FAILURE);
        }
    }
    unsigned char ch;
    bool c,quit;
s:;
    char co[3];
    system("cls");
    c = false;
    quit = false;
    gotoxy(0,2);
    cout << "--命令行模式--";
    gotoxy(0, 1);
    while (1) {
        cin.getline(co,3,'\n');
        if (co[0] == ':') {
            if (co[1] == 'q') {
                system("cls");
                exit(0);
            }
            if(co[1]=='s'){
                ofstream fout(argv[1]);
                int a=0;
                int b=0;
                while (1) {
                    b = 0;
                    while (1) {
                        if (buffer[a][0] == '\0') {
                            fout << '\n';
                            break;
                        }
                        fout << buffer[a][b];
                        if (buffer[a][b + 1] == '\0') { 
                            fout << '\n';
                            break; 
                        }
                        b++;
                    }
                    if (buffer[a + 1][0] == '~')break;
                    a++;
                }
                return 0;
            }
        }
        else if (co[0] == 'i') {
            break;
        }
        else {
            gotoxy(0, 1);
            printf("%c[2K", 27);
        }
    }
    system("cls");
    ifstream fin(argv[1]);
    string line;
    while (getline(fin, line)) // line中不包括每行的换行符
    {
        for (unsigned int i = 0; i < line.length(); i++) {
            buffer[h][i] = line[i];
            buffer[h][i + 1] = '\0';
        }
        pout(h);
        h++;
    }
    fin.close();
    unsigned char ch1;
    ch ='\0';
    ch1='\0';
    bool chc = false;
    while (1) {
        if (_kbhit()) {
            ch = '\0';
            ch = _getch();
            if (ch > 128&&!chc) {//汉字
                ch1 = _getch();
                chc = true;
            }
            else if (chc){
                chc = false;
                continue;
            }
            
            //操控
            if(ch==-32){
            }
            else if ( ch == 72) {//上
                gotoxy(0, h + 5);
                printf("%c[2K", 27);
                pout(h + 5);
                h--;
                if (h < 0)h++;
                if (pos > getlong(h))pos = getlong(h);
                gotoxy(pos,h);
            }
            else if ( ch == 80) {//下
                gotoxy(0, h + 5);
                printf("%c[2K", 27);
                pout(h + 5);
                h++;
                if (h > MAX)h = MAX;
                if (pos > getlong(h))pos = getlong(h);
                gotoxy(pos, h);
            }
            else if ( ch == 75) {//左
                pos--;
                if (buffer[h][pos] > 128)pos--;
                if (pos < 0)pos = 0;
                gotoxy(pos, h);
            }
            else if ( ch == 77) {//右
                pos++;
                if (buffer[h][pos] > 128)pos++;
                if (buffer[h][pos]=='\0')pos--;
                gotoxy(pos, h);
            }
            else if (ch == 27) { // ESC
                break;
            }
                else if (ch == 13) {//换行
                    gotoxy(0, h + 5);
                    printf("%c[2K", 27);
                    pout(h + 5);
                    h++;
                    if (h > MAX)h = MAX;
                    pos = 0;
                    if (buffer[h][0] == '~' ) { 
                        buffer[h][0] = '\0';
                    }
                    else {
                        move_d(h);
                    }
                    pout(h ,-1 ,true);
                }
                else if (ch == 8) {//退格符
                    if (pos >= 1) {
                        if(buffer[h][pos]>128){//汉字
                            move_l(h, pos);
                            pos--;
                            move_l(h, pos);
                            pos--;
                        }
                        else {//非汉字
                            move_l(h, pos);
                            pos--;
                            pout(h, pos);
                        }
                    }
                }
                else {//读入

                        if (buffer[h][pos + 2] != '\0') {//插入在中间
                            if (ch > 128) move_r(h, pos++, ch1);
                            move_r(h, pos++, ch);
                            pout(h);
                        }
                        else if (pos < MAX) {//插入在句尾
                            if (ch > 128)buffer[h][pos++] = ch1;
                            buffer[h][pos++] = ch;
                            buffer[h][pos + 1] = '\0';
                            pout(h);
                        }
                    
                }
                gotoxy(0, h + 5);
                printf("%c[2K", 27);
                cout << "(" << h << "," << pos << ")";
                gotoxy(pos, h);
        }
    }
    goto s;
}