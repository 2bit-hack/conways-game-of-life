#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>

using namespace std;

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;
const int RESOLUTION = 10;

int* temp;

const int rows = WIDTH/RESOLUTION;
const int cols = HEIGHT/RESOLUTION;

void initGrid(int* p)  //initializes grid with 0s and 1s randomly
{
    srand(time(NULL));
    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<cols; j++)
        {
            *(p+i*cols+j)=rand()%2;
        }
    }
}

int countNeighbors(int* p, int i, int j, int s) //reads 2D location from current gen and returns
                                         //number of live neighbors
{
    int sum = 0;
    switch(s)
    {
        case 0 : sum = *(p+i*cols+j+1) + *(p+(i+1)*cols+j) + *(p+(i+1)*cols+j+1); //top left corner
                 break;
        case 1 : sum = *(p+i*cols+j+1) + *(p+(i-1)*cols+j) + *(p+(i-1)*cols+j+1); //bottom left corner
                 break;
        case 2 : sum = *(p+i*cols+j-1) + *(p+(i+1)*cols+j) + *(p+(i+1)*cols+j-1); //top right corner
                 break;
        case 3 : sum = *(p+i*cols+j-1) + *(p+(i-1)*cols+j) + *(p+(i-1)*cols+j-1); //bottom right corner
                 break;
        case 4 : sum = *(p+(i-1)*cols+j) + *(p+(i-1)*cols+j+1) + *(p+(i)*cols+j+1) +
                       *(p+(i+1)*cols+j+1) + *(p+(i+1)*cols+j); //left edge
                 break;
        case 5 : sum = *(p+(i)*cols+j-1) + *(p+(i-1)*cols+j-1) + *(p+(i-1)*cols+j) +
                       *(p+(i-1)*cols+j+1) + *(p+(i)*cols+j+1); //bottom edge
                 break;
        case 6 : sum = *(p+(i-1)*cols+j) + *(p+(i-1)*cols+j-1) + *(p+(i)*cols+j-1) +
                       *(p+(i+1)*cols+j-1) + *(p+(i+1)*cols+j); //right edge
                 break;
        case 7 : sum = *(p+(i)*cols+j-1) + *(p+(i+1)*cols+j-1) + *(p+(i+1)*cols+j) +
                       *(p+(i+1)*cols+j+1) + *(p+(i)*cols+j+1); //top edge
                 break;
        case 8 : sum = *(p+(i-1)*cols+j-1) + *(p+(i-1)*cols+j) + *(p+(i-1)*cols+j+1) + *(p+i*cols+j-1) +
                       *(p+i*cols+j+1) + *(p+(i+1)*cols+j-1) + *(p+(i+1)*cols+j) + *(p+(i+1)*cols+j+1); //centered cells
                 break;

        default : cout << "Something terrible has happened somewhere and I'm too lazy to fix" << endl;
    }
    return sum;
}

int getState(int r, int c) //returns location of cell
{
    if(r==0 && c==0) //top left corner
        return 0;
    else if(r==rows-1 && c==0) //bottom left corner
        return 1;
    else if(r==0 && c==cols-1) //top right corner
        return 2;
    else if(r==rows-1 && c==cols-1) //bottom right corner
        return 3;
    else if(c==0) //left edge
        return 4;
    else if(r==rows-1) //bottom edge
        return 5;
    else if(c==cols-1) //right edge
        return 6;
    else if(r==0) //top edge
        return 7;
    else          //centered cells
        return 8;
}

void updateGrid(int* p, int* q)  //reads from current gen and writes to next gen
{
    int neighbors;int state;
    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<cols; j++)
        {
            state = getState(i, j);
            neighbors = countNeighbors(p, i, j, state);
            if((*(p+i*cols+j) == 1) && (neighbors<2 || neighbors>3)) //death by under/over population
                *(q+i*cols+j) = 0;
            else if((*(p+i*cols+j) == 0) && (neighbors == 3)) //resurrection by having 3 neighbors exactly
                *(q+i*cols+j) = 1;
            else if((*(p+i*cols+j) == 1) && (neighbors == 2 || neighbors == 3))  //cell has 2 or 3 neighbors, so it lives
                *(q+i*cols+j) = 1;
            else //dead cell remains dead
                *(q+i*cols+j) = 0;
        }
    }
}

int main()
{
    int curr_gen[rows][cols];
    int next_gen[rows][cols];

    int* curr = &curr_gen[0][0];
    int* next = &next_gen[0][0];

    initGrid(curr);

    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(RESOLUTION, RESOLUTION));

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Conway's Game of Life");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        updateGrid(curr, next);
        window.clear();
        for(int i=0; i<rows; i++)
        {
            for(int j=0; j<cols; j++)
            {
                if(*(next+i*cols+j))
                    rectangle.setFillColor(sf::Color::White);
                else
                    rectangle.setFillColor(sf::Color::Black);
                rectangle.setPosition(i*RESOLUTION, j*RESOLUTION);
                window.draw(rectangle);
            }
        }
        window.display();
        temp = next; //swapping pointers to next and current gens
        next = curr;
        curr = temp;

    }
    return 0;
}
