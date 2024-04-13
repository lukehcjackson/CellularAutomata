#include <SFML/Graphics.hpp>
#include <iostream>

enum CellStates { ALIVE, DEAD };

class Cell {
    private:
        CellStates state;

    public:
        Cell() {
            state = DEAD;
        }

        Cell(CellStates _state) {
            state = _state;
        }

        void setState(CellStates _state) {
            state = _state;
        }

        CellStates getState() {
            return state;
        }
};

//SFML window dimensions
int WindowWidth = 500;
int WindowHeight = 500;

//create the board
const int boardRows = 100;
const int boardCols = 100;
Cell board[boardRows][boardCols];

sf::Uint8* renderBoard(Cell board[boardRows][boardCols]) {
    sf::Uint8* pixels = new sf::Uint8[WindowWidth * WindowHeight * 4];

    int gridCols = WindowWidth / boardCols;
    int gridRows = WindowHeight / boardRows;

    //for every pixel in the window
    for (int x = 0; x < WindowWidth; x++) {
        for (int y = 0; y < WindowHeight; y++) {

            //find the index of the current pixel in the pixel array
            int index = (y + x * WindowWidth) * 4;

            int cellX = x / gridCols;
            int cellY = y / gridRows;

            if (board[cellY][cellX].getState() == ALIVE) {
                pixels[index] = 255;
                pixels[index + 1] = 255;
                pixels[index + 2] = 255;
                pixels[index + 3] = 255;
            } else {
                pixels[index] = 0;
                pixels[index + 1] = 0;
                pixels[index + 2] = 0;
                pixels[index + 3] = 255;
            }

        }

    }

    return pixels;

};

Cell (*GameOfLife(Cell board[boardRows][boardCols]))[boardRows][boardCols] {
    static Cell newBoard[boardRows][boardCols];

    //create a copy of the board to store the next timestep into
    for (int x = 0; x < boardCols; x++) {
        for (int y = 0; y < boardRows; y++) {
            newBoard[x][y] = board[x][y];
        }
    }

    for (int x = 0; x < boardCols; x++) {
        for (int y = 0; y < boardRows; y++) {
            //need to count live neighbours
            int liveNeighbours = 0;
            //look at neighbouring cells
            for (int posX = x-1; posX <= x+1; posX++) {
                for (int posY = y-1; posY <= y+1; posY++) {
                    //skip the current cell
                    if (posX == 0 && posY == 0) continue;
                    //handle edges
                    if (posX < 0 || posX > boardCols || posY < 0 || posY > boardRows) {
                        liveNeighbours++;
                        continue;
                    } 

                    liveNeighbours += (board[posX][posY].getState() == ALIVE);
                }
            }

            //change state based on number of live neighbours
            if (newBoard[x][y].getState() == ALIVE) {
                //'Any live cell with fewer than two live neighbors dies, as if by underpopulation.'
                if (liveNeighbours < 2) {
                    newBoard[x][y].setState(DEAD);
                }

                //'Any live cell with two or three live neighbors lives on to the next generation.'
                //This cell is already alive, so do nothing

                //'Any live cell with more than three live neighbors dies, as if by overpopulation.'
                if (liveNeighbours > 3) {
                    newBoard[x][y].setState(DEAD);
                }
                
            } else {
                //'Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.'
                if (liveNeighbours == 3) {
                    newBoard[x][y].setState(ALIVE);
                }
            }

        }
    }


    return &newBoard;
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "Cellular Automata");

    sf::Texture texture;
    texture.create(WindowWidth, WindowHeight);
    sf::Sprite sprite(texture);

    long long frameCount;

    //populate the board
    for (int x = 0; x < boardCols; x++) {
        for (int y = 0; y < boardRows; y++) {
            board[x][y] = Cell(ALIVE);
            if (x == 0 || x == boardCols - 1 || y == 0 || y == boardRows - 1) {
                board[x][y] = Cell(DEAD);
            }
        }
    }

    //print the board
    for (int x = 0; x < boardCols; x++) {
        for (int y = 0; y < boardRows; y++) {
            std::cout << board[x][y].getState() << " ";
        }
        std::cout << std::endl;
    }

    //runs once per frame
    while (window.isOpen())
    {
        //check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            //'close requested' event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        

        //draw background
        window.clear(sf::Color::Blue);

        //update texture
        sf::Uint8* pixels = renderBoard(board);

        //control framerate
        if (frameCount % 30 == 0) {
           //update the board based on whatever set of rules
            //CONWAY'S GAME OF LIFE
            Cell (*newBoard)[boardRows][boardCols] = GameOfLife(board);
            for (int x = 0; x < boardCols; x++) {
                for (int y = 0; y < boardRows; y++) {
                    board[x][y] = (*newBoard)[x][y];
                }
            } 
        }

        texture.update(pixels);
        //sprite.setTexture(texture);

        //draw texture (through the sprite)
        window.draw(sprite);

        //end the current frame
        window.display();
        frameCount++;
    }

    return 0;
}

// g++ *.cpp -I"C:\\msys64\\ucrt64\\include\\SFML" -o main -L"C:\\msys64\\ucrt64\\lib" -lsfml-graphics -lsfml-window -lsfml-system  