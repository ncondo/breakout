// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 525
#define WIDTH 600

// height and width of game's playing area
#define PLAYHEIGHT 455
#define PLAYWIDTH 540

// width of border
#define BORDER 30

// height and width of paddle
#define PHEIGHT 10
#define PWIDTH 60

// number of rows of bricks
#define ROWS 6

// number of columns of bricks
#define COLS 15

// radius of ball in pixels
#define RADIUS 5

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
GLabel initLivesLabel(GWindow window);
void gameOver(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
void updateLivesLabel(GWindow window, GLabel label, int lives);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);
    
    // set background to dark gray
    GRect background = newGRect(0, 0, WIDTH, HEIGHT);
    setFilled(background, true);
    setColor(background, "DARK_GRAY");
    add(window, background);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard
    GLabel pointsLabel = initScoreboard(window);
    
    // instantiate lives label
    GLabel livesLabel = initLivesLabel(window);
    
    // set top border to light gray
    GRect top_border = newGRect(0, 40, WIDTH, 30);
    setFilled(top_border, true);
    setColor(top_border, "LIGHT_GRAY");
    add(window, top_border);
    
    // set left border
    GRect left_border = newGRect(0, 40, 30, HEIGHT-40);
    setFilled(left_border, true);
    setColor(left_border, "LIGHT_GRAY");
    add(window, left_border);
    
    // set right border
    GRect right_border = newGRect(WIDTH-30, 40, 30, HEIGHT-40);
    setFilled(right_border, true);
    setColor(right_border, "LIGHT_GRAY");
    add(window, right_border);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    // seed pseudorandom number generator
    srand48(time(NULL));
    
    // initial velocity of ball
    double velocityX = drand48() + 2.0;
    double velocityY = drand48() + 2.0;
    
    // Store the object the ball has collided with
    GObject collisionObject;
    
    // wait for the user to click to start the game
    waitForClick();

    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        // check for mouse event
        GEvent event = getNextEvent(MOUSE_EVENT);

        // if we heard one
        if (event != NULL)
        {
            // if the event was mouse movement
            if (getEventType(event) == MOUSE_MOVED)
            {
                // ensure paddle follows cursor horizontally
                double x = getX(event) - PWIDTH / 2;
                double y = getY(paddle);
                setLocation(paddle, x, y);
            }
        }
        
        // create initial ball movement
        move(ball, velocityX, velocityY);
        
        // detect if the ball has collided with paddle or brick
        collisionObject = detectCollision(window, ball);
        
        if (collisionObject != NULL && strcmp(getType(collisionObject), "GRect") == 0)
        {
            if (collisionObject == paddle)
            {
                velocityX = -velocityX;
                velocityY = -velocityY;
            }
            else if (collisionObject != paddle && collisionObject != left_border && collisionObject != right_border && collisionObject != top_border && collisionObject != background)
            {
                velocityY = -velocityY;
                removeGWindow(window, collisionObject);
                bricks--;
                points++;
                updateScoreboard(window, pointsLabel, points);
            }
        }
        
        // bounce off right wall
        if (getX(ball) + 2*RADIUS >= WIDTH - BORDER)
        {
            velocityX = -velocityX;
        }
        // bounce off left wall
        else if (getX(ball) <= BORDER)
        {
            velocityX = -velocityX;
        }
        // bounce off ceiling
        else if (getY(ball) <= BORDER + 40)
        {
            velocityY = -velocityY;
        }
        // when user misses ball, reset ball and paddle and take away a life
        else if (getY(ball) + getHeight(ball) >= HEIGHT)
        {
            lives--;
            updateLivesLabel(window, livesLabel, lives);
            setLocation(ball, WIDTH/2 - RADIUS, HEIGHT/2 - RADIUS + 40);
            setLocation(paddle, (WIDTH-PWIDTH)/2, HEIGHT-(PHEIGHT*2));
            if (lives > 0)
            {
                waitForClick();
            } 
        }
        
        pause(10);
    }
    
    // display GAME OVER
    gameOver(window);

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    string brickColors[] = { "PINK", "MAGENTA", "ORANGE", "YELLOW", "GREEN", "CYAN" };
    int colorIndex = 0;
    int brickLevel = 125;
    
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
           GRect brick = newGRect(((WIDTH-60)/COLS) * j + 30, brickLevel, (WIDTH-60)/COLS, 15);
           setFilled(brick, true);
           setColor(brick, brickColors[colorIndex]);
           add(window, brick); 
        }
        colorIndex++;
        brickLevel += 15;
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    GOval ball = newGOval(WIDTH/2 - RADIUS, HEIGHT/2 - RADIUS + 40, RADIUS*2, RADIUS*2);
    setColor(ball, "LIGHT_GRAY");
    setFilled(ball, true);
    add(window, ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    GRect paddle = newGRect((WIDTH-PWIDTH)/2, HEIGHT-(PHEIGHT*2), PWIDTH, PHEIGHT);
    setFilled(paddle, true);
    setColor(paddle, "PINK");
    add(window, paddle);
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel pointsLabel = newGLabel("000");
    setFont(pointsLabel, "SansSerif-42");
    setColor(pointsLabel, "LIGHT_GRAY");
    setLocation(pointsLabel, 120, 35);
    add(window, pointsLabel);
    return pointsLabel;
}

/**
 * Instantiates, configures, and returns label for number of lives remaining.
 */
GLabel initLivesLabel(GWindow window)
{
    GLabel livesLabel = newGLabel("3");
    setFont(livesLabel, "SansSerif-42");
    setColor(livesLabel, "LIGHT_GRAY");
    setLocation(livesLabel, 420, 35);
    add(window, livesLabel);
    return livesLabel;
}

/**
 * Instantiates, configures, and returns label for number of lives remaining.
 */
void gameOver(GWindow window)
{
    GLabel gameOver = newGLabel("GAME OVER");
    setFont(gameOver, "SansSerif-42");
    setColor(gameOver, "RED");
    double x = (WIDTH - getWidth(gameOver)) / 2;
    double y = (HEIGHT - getHeight(gameOver) + 70) / 2;
    setLocation(gameOver, x, y);
    add(window, gameOver);
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%03i", points);
    setLabel(label, s);
}

/**
 * Updates label to keep track of number of lives remaining
 */
void updateLivesLabel(GWindow window, GLabel label, int lives)
{
    // update label
    char s[12];
    sprintf(s, "%i", lives);
    setLabel(label, s);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
