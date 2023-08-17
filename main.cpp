#include <iostream>
#include <cmath>
#include <windows.h>
#include <string.h>
#include <chrono>
#include <utility>
#include <ctype.h>
#include <vector>
#include <algorithm>

using namespace std;

int nScreenWidth = 120;
int nScreenHeight = 40;

//player's position and to run it smooth the float is used
float fPlayerX = 8.0f;
float fPlayerY = 8.0f;
float fPlayerA = 0.0f;      //angle of player's looking at

//map
int nMapHeight = 16;
int nMapWidth = 16;

//field of view
float fFOV = 3.14159f / 4.0f;
float fDepth = 16.0f;   //because map size is 16
float fSpeed = 5.0f;

int main()
{
    //This code creates a 2D screen of type wchar_t (something related to unicode)
    char *screen = new char[nScreenWidth*nScreenHeight];
    //text mode buffer
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0,  NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    //The buffer is the target of console
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    //creating map
    wstring map;
    map += L"################";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#.......####...#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#######........#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#.......########";
    map += L"#..............#";
    map += L"#..............#";
    map += L"################";

    //Time points for measuring duration
    auto tp1 = chrono::system_clock::now();
    auto tp2 = chrono::system_clock::now();

    //Game loop
    while(1){

        //For each iteration of game loop
        tp2 = chrono::system_clock::now();                      //Grabbing current system time
        chrono::duration<float> elapsedTime = tp2 - tp1;        //calculating the duration between current and previous system time
        tp1 = tp2;                                              //updating the old time point
        float fElapsedTime = elapsedTime.count();               //Elapsed time in floating point because rest of the game is in floating point


        //Controls
        //Handles CCW Rotation
        if(GetAsyncKeyState((unsigned short)'A') & 0x8000)  //For turning left
            fPlayerA -= (fSpeed * 0.8f) * fElapsedTime;

        if(GetAsyncKeyState((unsigned short)'D') & 0x8000)  //For turning right
            fPlayerA += (fSpeed * 0.8f) * fElapsedTime;

        //For walking forward
        if(GetAsyncKeyState((unsigned short)'W') & 0x8000){

            fPlayerX += sinf(fPlayerA) * fSpeed * fElapsedTime;
            fPlayerY += cosf(fPlayerA) * fSpeed * fElapsedTime;

            //collision detection for hitting the wall ahead
            if(map.c_str()[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#'){
                fPlayerX -= sinf(fPlayerA) * fSpeed * fElapsedTime;
                fPlayerY -= cosf(fPlayerA) * fSpeed * fElapsedTime;
            }
        }

        //For going backwards
        if(GetAsyncKeyState((unsigned short)'S') & 0x8000){
            fPlayerX -= sinf(fPlayerA) * fSpeed * fElapsedTime;
            fPlayerY -= cosf(fPlayerA) * fSpeed * fElapsedTime;

            //Collision detection for hitting the wall behind
            if(map.c_str()[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#'){
                fPlayerX += sinf(fPlayerA) * fSpeed * fElapsedTime;
                fPlayerY += cosf(fPlayerA) * fSpeed * fElapsedTime;
            }
        }

        //Potter's Algorithm
        //this loop goes through 120 columns
        for(int x = 0; x < nScreenWidth; x++){
            //for each column, calculating the projected ray angle into world space
            float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) * fFOV;

            //Distance of the player from the wall
            float fDistanceToWall = 0.0f;

            //Increment size for ray casting
            float fStepSize = 0.1f;

            //Flag to check player hitting the wall
            bool bHitWall = false;

            //Flag for hitting the boundary
            bool bBoundary = false;

            //To calculate test point
            //Unit vectors for ray in player space
            //Represents which direction player's looking in
            float fEyeX = sinf(fRayAngle);
            float fEyeY = cosf(fRayAngle);

            while(!bHitWall && fDistanceToWall < fDepth){       //maximum limit to check how far the player goes in case of there is no wall ahead

                //incrementing distance by small steps towards the wall
                fDistanceToWall += fStepSize;

                //Creating line of given distance towards the player looking at, using the unit vectors, extending unit vector
                int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
                int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);

                //we have int location and now we can test if the ray is out of bound
                if(nTestX <0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight){

                    bHitWall = true;                //Just setting distance to maximum depth
                    fDistanceToWall = fDepth;

                }
                //checking the cells individually of the map
                else{
                    //converting the 2D system into 1D for the array
                    //Ray is inbounds so test to see if the ray cell is a wall block
                    if(map.c_str()[nTestY * nMapWidth + nTestX] == '#'){

                        bHitWall = true;

                        vector<pair<float, float>> p;   //distance, dot

                        for(int tx = 0; tx < 2; tx++)
                            for(int ty = 0; ty < 2; ty++){

                                float vy = (float)nTestY +  ty -fPlayerY;
                                float vx = (float)nTestX +  tx -fPlayerX;
                                float d = sqrt(vx*vx + vy*vy);
                                float dot = (fEyeX * vx / d) + (fEyeY * vy / d);
                                p.push_back(make_pair(d, dot));

                            }

                        sort(p.begin(), p.end(), [](const pair<float, float> &left, const pair<float, float> &right) {return left.first < right.first;});

                        float fBound = 0.01;
                        if(acos(p.at(0).second) < fBound) bBoundary = true;
                        if(acos(p.at(1).second) < fBound) bBoundary = true;
                        if(acos(p.at(2).second) < fBound) bBoundary = true;

                    }
                }
            }

            //Calculating the distance to ceiling and floor
            int nCeiling = (float)(nScreenHeight / 2.0) -  nScreenHeight / ((float)fDistanceToWall);
            int nFloor = nScreenHeight - nCeiling;

            //For shading the walls
            short nShade = ' ';
            if(fDistanceToWall <= fDepth / 4.0f)         nShade = 0x2588;   //very close
            else if(fDistanceToWall < fDepth / 3.0f)     nShade = 0x2593;
            else if(fDistanceToWall < fDepth / 2.0f)     nShade = 0x2592;
            else if(fDistanceToWall < fDepth)            nShade = 0x2591;
            else                                         nShade = ' ';      //Too far

            if(bBoundary)   nShade = ' ';   //Black it out

            //Drawing the ceiling, wall and floor in a column
            for(int y = 0; y < nScreenHeight; y++){

                if(y <= nCeiling)
                    screen[y*nScreenWidth + x] = ' ';
                else if(y > nCeiling && y <= nFloor)
                    screen[y*nScreenWidth + x] = nShade;
                else{

                    //For shading the floor
                    float b = 1.0f - (((float)y - nScreenHeight / 2.0f) / (float)nScreenHeight / 2.0f);
                    if(b < 0.25)         nShade = '#';
                    else if(b < 0.5)     nShade = '*';
                    else if(b < 0.75)    nShade = '.';
                    else if(b < 0.9)     nShade = '-';
                    else                 nShade = ' ';
                    screen[y*nScreenWidth + x] = nShade;

                }

            }

        }

        //Drawing map
        for(int nx = 0; nx < nMapHeight; nx++)
            for(int ny = 0; ny < nMapHeight; ny++){

                screen[(ny + 1)*nScreenWidth + nx] = map[ny * nMapWidth + nx];

            }

        //Marker showing where player is
        screen[((int)fPlayerY + 1) * nScreenWidth + (int)fPlayerX] = 'P';

        //write to screen
        screen[nScreenWidth * nScreenHeight - 1] = '\0';       // where to stop - the end of the screen array which is a character - '\0'
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);
    }

    return 0;
}
