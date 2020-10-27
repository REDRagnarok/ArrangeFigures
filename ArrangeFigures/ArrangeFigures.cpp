// ArrangeFigures.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <windows.h>
#include <clocale>
#include <conio.h>
#include <vector>
#include <random>
#include <time.h>
using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");

    HANDLE hcw = GetStdHandle(STD_OUTPUT_HANDLE);
    short width = 100;
    short height = 40;
    COORD screenSize = { width,height };
    SMALL_RECT canvasSize = { 0,0,screenSize.X - 1,screenSize.Y - 1 };
    SetConsoleScreenBufferSize(hcw, screenSize);
    SetConsoleWindowInfo(hcw, true, &canvasSize);

    HWND hwd = GetConsoleWindow();
    HDC wdc = GetDC(hwd);

    HPEN pen = CreatePen(PS_SOLID, 2, RGB(128, 57, 128));
    COLORREF bgcolor = GetPixel(wdc, 1, 1);
    HBRUSH brush = CreateSolidBrush(bgcolor);

    SelectObject(wdc, pen);
    SelectObject(wdc, brush);

    RECT bound;
    GetClientRect(hwd, &bound);
    int topLeftX = bound.right / 2 - 150;
    int topLeftY = bound.bottom / 2 - 200;
    int bottomRightX = bound.right / 2 + 150;
    int bottomRightY = bound.bottom / 2 + 200;


    RECT glass = { topLeftX, topLeftY, bottomRightX, bottomRightY };
    int glassArea = (bottomRightX - topLeftX) * (bottomRightY - topLeftY);
    int tilesArea = 0;

    HRGN field = CreateRectRgnIndirect(&glass);
    FillRgn(wdc, field, brush);

    FrameRect(wdc, &glass, CreateSolidBrush(RGB(128, 57, 128)));

    vector<RECT> figs;
    figs.push_back(glass);
    RECT RectFig = { 0,0,0,0 };
    srand(time(0));

   

    for (int i = 0; i < 50000; i++)
    {
        // Задаем случайным образом координаты очередного квадрата
        int rangeX = topLeftX + 1 + rand() % (bottomRightX - topLeftX - 11);
        int rangeY = topLeftY - 1 + rand() % (bottomRightY - topLeftY - 11);

        // Случайный выбор типа фигуры
        int figChoice = rand() % 3;

        switch (figChoice)
        {
        case 1: RectFig = { rangeX,rangeY,rangeX + 10,rangeY + 10 };
              break;
        case 2: RectFig = { rangeX,rangeY,rangeX + 25,rangeY + 25 };
              break;
        default: RectFig = { rangeX,rangeY,rangeX + 50,rangeY + 25 };
              break;
        }

        // Проверка, что фигура внутри стакана
        if (PtInRegion(field, RectFig.left, RectFig.top) && PtInRegion(field, RectFig.right, RectFig.bottom))
        {
            // Проверка, что нет наложения с другими фигурами 
            POINT cornerOne = { RectFig.left, RectFig.top };
            POINT cornerTwo = { RectFig.right,RectFig.bottom };
            POINT cornerThree = { RectFig.left,RectFig.bottom };
            POINT cornerFour = { RectFig.right,RectFig.top };

            if (figs.size() != 1)
            {
                bool isOverlapped = false;

                for (int f = 1; f < figs.size(); f++)
                {

                    //Проверка на пересечение с другими прямоугольниками
                    if (PtInRect(&figs[f], cornerOne) | PtInRect(&figs[f], cornerTwo) | PtInRect(&figs[f], cornerThree) | PtInRect(&figs[f], cornerFour))
                    {
                        isOverlapped = true;
                        break;
                    }
                }
                                
                if (isOverlapped) 
                { 
                    continue; 
                }
                else
                {
                    // Проверка на наличие фигур внутри нового прямоугольника
                    HRGN outerRect = CreateRectRgnIndirect(&RectFig);
                    for (int j = 1; j < figs.size(); j++) 
                    {
                        if (RectInRegion(outerRect, &figs[j]))
                        {
                            isOverlapped = true;
                            break;
                        }
                    }
                    if (isOverlapped)
                    {
                        continue;
                    }
                    else 
                    {
                        FrameRect(wdc, &RectFig, CreateSolidBrush(RGB(128, 57, 128)));
                        figs.push_back(RectFig);
                        switch (figChoice) 
                        {
                        case 1: tilesArea += 100;
                            break;
                        case 2: tilesArea += 625;
                            break;
                        case 3: tilesArea += 1250;
                            break;
                        default:
                            break;
                        }
                    }
                   
                }
            }
            else
            {
                FrameRect(wdc, &RectFig, CreateSolidBrush(RGB(128, 57, 128)));
                figs.push_back(RectFig);
                switch (figChoice)
                {
                case 1: tilesArea += 100;
                    break;
                case 2: tilesArea += 625;
                    break;
                case 3: tilesArea += 1250;
                    break;
                default:
                    break;
                }
            }

        }
                      
    }// for end
    cout << "Glass area: " << glassArea << " " << "  Tiles area: " << tilesArea;
    _getch();
}


