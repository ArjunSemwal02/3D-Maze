#include <iostream>
#include <windows.h>


using namespace std;


int nScreenWidth = 120;
int nScreenHeight = 40;

//player's position and to run it smooth the float is used
float fPlayerX = 0.0f;
float fPlayery = 0.0f;
//angle of player's looking at
float fPlayerA = 0.0f;


//map
int nMapHeight = 16;
int nMapWidth = 16;

//field of view
float fFOV = 3.14159 / 4.0;

int main()
{
    //This code creates a 2D screen of type wchar_t (something related to unicode)
    wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight];

    //text mode buffer
    HANDLE hconsole CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE 0,  NULL, CONSOLE_TEXTMODE_BUFFER, NULL)

    //The buffer is the target of console
    SetConsoleActiveScreenBuffer(hconsole);

    DWORD dwBytesWritten = 0;


    //creating map
    wstring = map;

    map += L"################";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"################";



    //Game loop
    while(){


        //Potter's Algorithm
        //this loop goes through 120 columns
        for(int x = 0; x < nScreenWidth; x++){

            //for each column, calculating the projected ray angle into world space
            float fRayAngle = (fPlayerA - fFOV / 2) + ((float)x / (float))



        }


        //write to screen
        screen[nScreenWidth * nScreenHeight - 1] = '\0';        // where to stop - the end of the screen array which is a character - '\0'
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth*nScreenHeight, {0,0}, &dwBytesWritten);
    }








    return 0;
}
