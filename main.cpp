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
const int boardRows = 10;
const int boardCols = 10;
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

int main()
{
    sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "Cellular Automata");

    sf::Texture texture;
    texture.create(WindowWidth, WindowHeight);
    sf::Sprite sprite(texture);

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
        texture.update(pixels);
        //sprite.setTexture(texture);

        //draw texture (through the sprite)
        window.draw(sprite);

        //end the current frame
        window.display();
    }

    return 0;
}

// g++ *.cpp -I"C:\\msys64\\ucrt64\\include\\SFML" -o main -L"C:\\msys64\\ucrt64\\lib" -lsfml-graphics -lsfml-window -lsfml-system  