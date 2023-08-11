#include <iostream>
#include <graphics.h>
#include <cmath>
#include <ctime>
#include <string>

//to play audio
#include <Windows.h>

using namespace std;

const double PI = 3.14159265358979323846;

struct HourTimeText {
    int x;
    int y;
};

struct LineData {
    int x1;
    int y1;
    int x2;
    int y2;
};

char* getCurrentTimeAsString() {
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    char* dateString = new char[12];

    strftime(dateString, 12, "%I:%M:%S %p", timeinfo);
    return dateString;
}

int getHourFromDateString(char* dateString) {
    string s(dateString);
    string hour = s.substr(0, 2);
    return stoi(hour);
}

int getMinFromDateString(char* dateString) {
    string s(dateString);
    string minute = s.substr(3, 2);
    return stoi(minute);
}

int getSecFromDateString(char* dateString) {
    string s(dateString);
    string sec = s.substr(6, 2);
    return stoi(sec);
}

void calculateHourLineBasedOnHour(LineData* line, int hour, int centerX, int centerY, int r) {
    double angleTilt = (hour - 3) * (2 * PI / 12);
    double fromCenter = r * 0.4;
    int endX = centerX + fromCenter * cos(angleTilt);
    int endY = centerY + fromCenter * sin(angleTilt);

    line->x1 = centerX;
    line->x2 = endX;
    line->y1 = centerY;
    line->y2 = endY;
}

void calculateMinLineBasedOnMin(LineData* line, int minut, int centerX, int centerY, int r) {
    double fromZero = (270 * PI)/ 180; //was supposed to be 90 degree but 270 to combat inversion
    double angleTilt =  fromZero + abs((minut) * (2 * PI / 60));
    double fromCenter = r * 0.6;
    int endX = centerX + fromCenter * cos(angleTilt);
    int endY = centerY + fromCenter * sin(angleTilt);

    line->x1 = centerX;
    line->x2 = endX;
    line->y1 = centerY;
    line->y2 = endY;
}

void calculateSecondLineBasedOnSec(LineData* line, int sec, int centerX, int centerY, int r) {
    double fromZero = (270 * PI) / 180;
    double angleTilt = fromZero + abs((sec) * (2 * PI / 60));
    double fromCenter = r * 0.8;
    int endX = centerX + fromCenter * cos(angleTilt);
    int endY = centerY + fromCenter * sin(angleTilt);

    line->x1 = centerX;
    line->x2 = endX;
    line->y1 = centerY;
    line->y2 = endY;
}

void calculateHourCoordinate(HourTimeText* coordinates, int centerX, int centerY, int r) {
    double fromZero = (3 * PI) / 2; //was supposed to be 90 degree but 270 to combat inversion
    double angleInc = (2 * PI) / 12;
    double startingAngle = fromZero + angleInc;
    double fromCenter = r * 0.9;

    for (int i = 0; i < 12; ++i) {
        double angle = startingAngle + (i * angleInc);
        int x = centerX + fromCenter * cos(angle);
        int y = centerY + fromCenter * sin(angle);

        coordinates[i].x = x;
        coordinates[i].y = y;
    }
}

void renderWindowOneContent(int winOne, int w, int h) {
    setcurrentwindow(winOne);
    int centerX = w/2, centerY = h/2;
    int radius = 200;


    HourTimeText hourCoordinates[12];
    calculateHourCoordinate(hourCoordinates, centerX, centerY, radius);

    while(true) {
        circle(centerX, centerY, 3);
        setcolor(BLUE);
        circle(centerX, centerY, radius);
        circle(centerX, centerY, (radius + 10));

        setcolor(WHITE);
        settextstyle(8, 0, 3);

        for (int i = 0; i < 12; ++i) {
            //converting the loop index to a text.
            string indexStr = to_string(i + 1);
            char* charPtr = strdup(indexStr.c_str());
            outtextxy(hourCoordinates[i].x, hourCoordinates[i].y, charPtr);
        }

        int hr = getHourFromDateString(getCurrentTimeAsString());
        int m = getMinFromDateString(getCurrentTimeAsString());
        int s = getSecFromDateString(getCurrentTimeAsString());

        LineData hourLine, minLine, secLine;
        calculateHourLineBasedOnHour(&hourLine, hr, centerX, centerY, radius);
        line(hourLine.x1, hourLine.y1, hourLine.x2, hourLine.y2);

        setcolor(RED);
        calculateMinLineBasedOnMin(&minLine, m, centerX, centerY, radius);
        line(minLine.x1, minLine.y1, minLine.x2, minLine.y2);

        setcolor(YELLOW);
        calculateSecondLineBasedOnSec(&secLine, s, centerX, centerY, radius);
        line(secLine.x1, secLine.y1, secLine.x2, secLine.y2);

        setcolor(WHITE);
        outtextxy((centerX - (radius / 5)), (centerY + radius + 25), getCurrentTimeAsString());
        PlaySound(TEXT("clock_tick_2.wav"), nullptr, SND_FILENAME | SND_ASYNC);

        // sleep for 1 sec
        delay(1000);
        cleardevice();
    }
}

void renderWindowTwoContent(int winTwo) {
    setcurrentwindow(winTwo);
    settextstyle(8, 0, 4);
    outtextxy(10, 10, "Group Members");
    settextstyle(8, 0, 2);
    outtextxy(10, 50, "Group Members");
    outtextxy(10, 90, "Group Members");
    outtextxy(10, 130, "Group Members");
    outtextxy(10, 170, "Group Members");
    outtextxy(10, 210, "Group Members");
    outtextxy(10, 250, "Group Members");
}

int main()
{
    int gd = DETECT, gm;
    int h = (480 + 50), w = (640 + 50);
    int windowOne, windowTwo;
    initgraph(&gd, &gm, "");
    const char* title="CS3 - CG Final Assignment";
    const char* group_members = "Group Members";
    windowOne = initwindow(w, h, title);
    windowTwo = initwindow((w/2), h);

    renderWindowTwoContent(windowTwo);
    //draw on the first window
    renderWindowOneContent(windowOne, w, h);

    getch();
    closegraph();
    return 0;
}
