//
// breakout.c
//
// Computer Science 50
// Problem Set 3
//

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

// height and width of paddle
#define PHEIGHT 10
#define PWIDTH 60

// number of rows of bricks
#define ROWS 6

// number of columns of bricks
#define COLS 15

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
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

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;

    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        // TODO
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
    }

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
    // TODO
    for (int i = 0; i < COLS; i++)
    {
        GRect pink = newGRect((WIDTH/COLS) * i, 100, WIDTH/COLS, 15);
        setFilled(pink, true);
        setColor(pink, "PINK");
        add(window, pink);
    }
    
    for (int i = 0; i < COLS; i++)
    {
        GRect magenta = newGRect((WIDTH/COLS) * i, 115, WIDTH/COLS, 15);
        setFilled(magenta, true);
        setColor(magenta, "MAGENTA");
        add(window, magenta);
    }
    
    for (int i = 0; i < COLS; i++)
    {
        GRect orange = newGRect((WIDTH/COLS) * i, 130, WIDTH/COLS, 15);
        setFilled(orange, true);
        setColor(orange, "ORANGE");
        add(window, orange);
    }
    
    for (int i = 0; i < COLS; i++)
    {
        GRect yellow = newGRect((WIDTH/COLS) * i, 145, WIDTH/COLS, 15);
        setFilled(yellow, true);
        setColor(yellow, "YELLOW");
        add(window, yellow);
    }
    
    for (int i = 0; i < COLS; i++)
    {
        GRect green = newGRect((WIDTH/COLS) * i, 160, WIDTH/COLS, 15);
        setFilled(green, true);
        setColor(green, "GREEN");
        add(window, green);
    }
    
    for (int i = 0; i < COLS; i++)
    {
        GRect cyan = newGRect((WIDTH/COLS) * i, 175, WIDTH/COLS, 15);
        setFilled(cyan, true);
        setColor(cyan, "CYAN");
        add(window, cyan);
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    // TODO
    return NULL;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    // TODO
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
    // TODO
    return NULL;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
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
