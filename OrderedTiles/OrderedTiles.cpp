// ArrangeFigures.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <windows.h>
#include <clocale>
#include <conio.h>
#include <vector>
#include <random>
#include <time.h>
#include <cstdio>
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
    int bottomRightY = bound.bottom / 2 + 201;


    RECT glass = { topLeftX, topLeftY, bottomRightX, bottomRightY };
    int glassArea = (bottomRightX - topLeftX) * (bottomRightY - topLeftY);
    int tilesArea = 0;

    HRGN field = CreateRectRgnIndirect(&glass);
    FillRgn(wdc, field, brush);
    srand(time(0));
    
    int hsizes[] = { 10,25,25 };
    int lsizes[] = { 10,25,50 };
    int squares[] = { 100,625,1250 };
    RECT RectFig = { 0,0,0,0 };
    vector<RECT> figs;
    vector<RECT> tSet;

    bool reachTheBottom = false;
    int fType;
    
    cout << "Задайте желаемый максимальный процент заполнения( целое число < 100): " << endl;
    double density = 0;
    cin >> density;
    density = (density / 100) * glassArea;
    
    while (tilesArea < density)
    {
        
        tilesArea = 0;
        RectFig = { 0,0,0,0 };
        figs.erase(figs.begin(), figs.end());
        tSet.erase(tSet.begin(), tSet.end());
            
        do
        {
            // Если укладываем первую фигуру то берем начальные значения
            if (RectFig.right == 0)
            {
                RectFig = { topLeftX,topLeftY,topLeftX + lsizes[1],topLeftY + lsizes[1] };
                tilesArea += squares[1];
                tSet.push_back(RectFig);
                //FrameRect(wdc, &RectFig, CreateSolidBrush(RGB(128, 57, 128))); // И рисуем нашу фигуру
                figs.push_back(RectFig);//Сохраняем крайнюю левую фигуру каждого ряда в массив

            }
            else
            {// Иначе переносим точку отрисовки новой фигуры вправо по оси X
                RectFig.left = RectFig.right;
                if (PtInRegion(field, RectFig.left, RectFig.top)) //Проверям, что не вышли за правую границу
                {
                    if (!reachTheBottom) {
                        fType = rand() % 3;// Случайно выбираем тип фигуры из массива фигур
                    }
                    else
                    {
                        fType = 0;
                    }
                    RectFig = { RectFig.left,RectFig.top,RectFig.left + lsizes[fType],RectFig.top + hsizes[fType] }; // Уточняем координаты фигуры

                    if (PtInRegion(field, RectFig.right, RectFig.bottom))
                    {
                        if (fType == 0)// Если понимаем, что наша фигура - малый квадрат, под ним рисуем еще один.
                        {
                            tSet.push_back(RectFig);
                            //FrameRect(wdc, &RectFig, CreateSolidBrush(RGB(128, 57, 128)));
                            RECT Double = { RectFig.left,RectFig.top + hsizes[fType],RectFig.left + lsizes[fType],RectFig.top + 2 * hsizes[fType] };
                            tSet.push_back(Double);
                            //FrameRect(wdc, &Double, CreateSolidBrush(RGB(128, 57, 128)));
                            tilesArea += 2 * squares[0];
                        }
                        // Если фигура не вылазит за правый край стакана или нижний край
                        tSet.push_back(RectFig);
                        //FrameRect(wdc, &RectFig, CreateSolidBrush(RGB(128, 57, 128))); // Рисуем её
                        tilesArea += squares[fType];
                    }
                    else
                    {
                        RectFig.left = topLeftX;
                        RectFig.right = RectFig.left + lsizes[fType];
                        if (!figs.empty())
                        {
                            RectFig.top = figs[figs.size() - 1].top + hsizes[2];
                            RectFig.bottom = RectFig.top + hsizes[fType];
                            if (glass.bottom < RectFig.bottom) { break; }
                            tilesArea += squares[fType];
                            tSet.push_back(RectFig);
                            //FrameRect(wdc, &RectFig, CreateSolidBrush(RGB(128, 57, 128))); // И рисуем нашу фигуру
                            figs.push_back(RectFig);//Сохраняем крайнюю левую фигуру каждого ряда в массив
                        }

                    }
                }

            }

        } while (PtInRegion(field, RectFig.right, RectFig.bottom));
        
    }
    double fact = ((double(tilesArea)) / (double)glassArea * (double)100);
    cout << "Площадь стакана: " << glassArea << "  " << "Площадь фигур: " << tilesArea << "  " << endl;
    cout << "Процент заполнения: " << " заданный: " << density << " фактический: " << fact << endl;
    
    
    FrameRect(wdc, &glass, CreateSolidBrush(RGB(128, 57, 128)));
    for (RECT &tile : tSet) 
    {
        FrameRect(wdc, &tile, CreateSolidBrush(RGB(128, 57, 128)));
    }

    _getch();

    ReleaseDC(hwd, wdc);
 }
    
     


