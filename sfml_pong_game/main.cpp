#include <iostream>
#include <SFML/Graphics.hpp>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float PADDLE_SPEED = 0.4f;
const float BALL_SPEED = 0.3f;
const float BALL_RADIUS = 10;
const int MAX_SCORE = 5;

// Function to handle player movement
void handlePlayerMovement(sf::RectangleShape& paddle) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && paddle.getPosition().y > 0) {
        paddle.move(0, -PADDLE_SPEED);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && paddle.getPosition().y + paddle.getSize().y < SCREEN_HEIGHT) {
        paddle.move(0, PADDLE_SPEED);
    }
}

// Function to handle AI movement
void handleAIMovement(sf::RectangleShape& paddle, const sf::CircleShape& ball) {
    // Track the ball's position
    float ballY = ball.getPosition().y;
    float paddleY = paddle.getPosition().y;

    // Move the paddle towards the ball
    if (ballY < paddleY && paddle.getPosition().y > 0) {
        paddle.move(0, -PADDLE_SPEED);
    }
    if (ballY > paddleY && paddle.getPosition().y + paddle.getSize().y < SCREEN_HEIGHT) {
        paddle.move(0, PADDLE_SPEED);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Pong Game");

    // Create paddles and ball
    sf::RectangleShape leftPaddle(sf::Vector2f(20, 100));
    sf::RectangleShape rightPaddle(sf::Vector2f(20, 100));
    sf::CircleShape ball(BALL_RADIUS, 10);

    leftPaddle.setFillColor(sf::Color::Red);
    rightPaddle.setFillColor(sf::Color::Red);
    ball.setFillColor(sf::Color::Yellow);

    leftPaddle.setPosition(50, SCREEN_HEIGHT / 2 - leftPaddle.getSize().y / 2);
    rightPaddle.setPosition(SCREEN_WIDTH - 50 - rightPaddle.getSize().x, SCREEN_HEIGHT / 2 - rightPaddle.getSize().y / 2);
    ball.setPosition(SCREEN_WIDTH / 2 - BALL_RADIUS, SCREEN_HEIGHT / 2 - BALL_RADIUS);

    sf::Vector2f ballVelocity(BALL_SPEED, BALL_SPEED);

    int leftScore = 0;
    int rightScore = 0;

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cout << "error loading font file" << std::endl;
        return 1;
    }

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(25);
    scoreText.setPosition(SCREEN_WIDTH / 2 - scoreText.getGlobalBounds().width / 2, 10);
    scoreText.setFillColor(sf::Color::White);

    bool gameOver = false;
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(30);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition(SCREEN_WIDTH / 4 - gameOverText.getGlobalBounds().width / 4, SCREEN_HEIGHT / 3 - gameOverText.getGlobalBounds().height / 3);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::R && gameOver) {
                    // Restart the game
                    leftScore = 0;
                    rightScore = 0;
                    gameOver = false;
                    ball.setPosition(SCREEN_WIDTH / 2 - BALL_RADIUS, SCREEN_HEIGHT / 2 - BALL_RADIUS);
                }
            }
        }

        if (!gameOver) {
            // Paddle movement
            handlePlayerMovement(leftPaddle);
            handleAIMovement(rightPaddle, ball);

            // Ball movement and collision
            ball.move(ballVelocity);

            // Ball collision with top and bottom walls
            if (ball.getPosition().y <= 0 || ball.getPosition().y + BALL_RADIUS * 2 >= SCREEN_HEIGHT) {
                ballVelocity.y = -ballVelocity.y;
            }

            // Ball collision with left and right walls
            if (ball.getPosition().x <= 0) {
                ballVelocity.x = -ballVelocity.x;
                rightScore++;
            }
            if (ball.getPosition().x + BALL_RADIUS * 2 >= SCREEN_WIDTH) {
                ballVelocity.x = -ballVelocity.x;
                leftScore++;
            }

            // Ball collision with paddles
            if (ball.getGlobalBounds().intersects(leftPaddle.getGlobalBounds()) ||
                ball.getGlobalBounds().intersects(rightPaddle.getGlobalBounds()))
            {
                ballVelocity.x = -ballVelocity.x;
            }


            // Check if game over
            if (leftScore >= MAX_SCORE || rightScore >= MAX_SCORE) 
            {
                if (leftScore >= MAX_SCORE)
                {
                    gameOverText.setString("Left Won! Press R to restart.");
                }
                else if (rightScore >= MAX_SCORE)
                {
                    gameOverText.setString("Right Won! Press R to restart.");
                }

                gameOver = true;
            }
        }

        // Update score text
        scoreText.setString(std::to_string(leftScore) + "  :  " + std::to_string(rightScore));

        window.clear(sf::Color::Blue);

        // Draw paddles and ball
        window.draw(leftPaddle);
        window.draw(rightPaddle);
        window.draw(ball);
        window.draw(scoreText);

        // Draw game over text if game over
        if (gameOver) {
            window.draw(gameOverText);
        }

        window.display();
    }

    system("pause");
}