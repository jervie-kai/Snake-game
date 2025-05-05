#include <SFML/Graphics.hpp>
#include <deque>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace sf;
using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;
const int SIZE = 20;
const int ROWS = HEIGHT / SIZE;
const int COLS = WIDTH / SIZE;

struct Segment {
    int x, y;
};

enum Direction { UP, DOWN, LEFT, RIGHT };

class SnakeGame {
private:
    RenderWindow window;
    deque<Segment> snake;
    Direction direction;
    Segment food;
    int score;
    bool gameOver;
    Font font;
    Text text;
    string playerName;

    Texture imageTexture;
    Sprite imageSprite;

    void handleEvents() {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::KeyPressed) {
                if (!gameOver) {
                    if (e.key.code == Keyboard::Up && direction != DOWN) direction = UP;
                    else if (e.key.code == Keyboard::Down && direction != UP) direction = DOWN;
                    else if (e.key.code == Keyboard::Left && direction != RIGHT) direction = LEFT;
                    else if (e.key.code == Keyboard::Right && direction != LEFT) direction = RIGHT;
                }

                if (e.key.code == Keyboard::Enter && gameOver) {
                    reset();
                }
            }
        }
    }

    void update() {
        Segment head = snake.front();
        switch (direction) {
            case UP: head.y--; break;
            case DOWN: head.y++; break;
            case LEFT: head.x--; break;
            case RIGHT: head.x++; break;
        }

        if (head.x < 0 || head.y < 0 || head.x >= COLS || head.y >= ROWS || collision(head)) {
            gameOver = true;
            return;
        }

        snake.push_front(head);
        if (head.x == food.x && head.y == food.y) {
            score++;
            spawnFood();
        } else {
            snake.pop_back();
        }
    }

    bool collision(Segment head) {
        for (size_t i = 1; i < snake.size(); ++i) {
            if (snake[i].x == head.x && snake[i].y == head.y) return true;
        }
        return false;
    }

    void draw() {
        window.clear();

        window.draw(imageSprite);

        // Snake
        for (auto& s : snake) {
        RectangleShape rectangle(Vector2f(SIZE - 2, SIZE - 2));
        float px = s.x * SIZE;
        float py = s.y * SIZE;

        rectangle.setPosition(px, py);
        rectangle.setFillColor(Color::Blue);

        window.draw(rectangle);
        }

        // Food
        CircleShape circle(SIZE / 2 - 2);
        circle.setFillColor(Color::Red);
        circle.setPosition(food.x * SIZE + 2, food.y * SIZE + 2);
        window.draw(circle);

        // Text
        text.setString(playerName + " | Score: " + to_string(score));
        text.setPosition(5, 5);
        window.draw(text);

        // Game Over
        if (gameOver) {
            Text over;
            over.setFont(font);
            over.setString("Game Over\nPress Enter to Restart");
            over.setCharacterSize(40);
            over.setFillColor(Color::Red);
            over.setPosition(WIDTH / 4, HEIGHT / 2 - 50);
            window.draw(over);
        }

        window.display();
    }

    void spawnFood() {
        srand(static_cast<unsigned>(time(0)));
        food.x = rand() % COLS;
        food.y = rand() % ROWS;
    }

    void reset() {
        snake.clear();
        snake.push_back({COLS / 2, ROWS / 2});
        score = 0;
        direction = RIGHT;
        gameOver = false;
        spawnFood();
    }
public:
    SnakeGame() : window(VideoMode(WIDTH, HEIGHT), "Snake Game by Jervie Paglinawan"), direction(RIGHT), score(0), gameOver(false) {
        font.loadFromFile("OpenSans-Regular.ttf");
        text.setFont(font);
        text.setCharacterSize(18);
        text.setFillColor(Color::White);

        playerName = "Player: Jervie Paglinawan";


        if (!imageTexture.loadFromFile("grass.png")) {
        }
        imageSprite.setTexture(imageTexture);
        imageSprite.setPosition(0, 30);

        imageSprite.setTexture(imageTexture);

        Vector2u textureSize = imageTexture.getSize();
        Vector2u windowSize = window.getSize();

        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

        imageSprite.setScale(scaleX, scaleY);


        spawnFood();
        snake.push_back({COLS / 2, ROWS / 2});
    }

    void run() {
        Clock clock;
        float timer = 0.0f, delay = 0.1f;

        while (window.isOpen()) {
            float time = clock.restart().asSeconds();
            timer += time;

            handleEvents();
            draw();
            if (!gameOver && timer > delay) {
                timer = 0;
                update();
            }


        }
    }
};

int main() {
    SnakeGame game;
    game.run();
    return 0;
}
