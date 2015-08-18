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
    
    // initial velocity of ball
    double velocityX = 2.0;
    double velocityY = 2.0;

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
        
        // create initial ball movement
        move(ball, velocityX, -velocityY);
        
        // detect if the ball has collided with paddle or brick
        GObject object = detectCollision(window, ball);
        
        // bounce off paddle
        if (object == paddle)
        {
            velocityY = drand48() + 2;
        }
        // bounce off right wall
        else if (getX(ball) + 2*RADIUS >= WIDTH - BORDER)
        {
            velocityX = -(drand48() + 2);
            //velocityY = velocityY;
        }
        // bounce off left wall
        else if (getX(ball) <= BORDER)
        {
            velocityX = drand48() + 2;
            //velocityY = velocityY;
        }
        // bounce off ceiling
        else if (getY(ball) <= BORDER + 40)
        {
            //velocityX = velocityX;
            velocityY = -(drand48() + 2);
        }
        // bounce off bottom (for testing only)
        else if (getY(ball) >= HEIGHT)
        {
            velocityY = drand48() + 2;
        }
        
        
        pause(10);
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
    
    for (int i = 0; i < COLS; i++)
    {
        GRect brick = newGRect(((WIDTH-60)/COLS) * i + 30, 125, (WIDTH-60)/COLS, 15);
        setFilled(brick, true);
        setColor(brick, "PINK");
        add(window, brick);
    }
    
    for (int i = 0; i < COLS; i++)
    {
        GRect brick = newGRect(((WIDTH-60)/COLS) * i + 30, 140, (WIDTH-60)/COLS, 15);
        setFilled(brick, true);
        setColor(brick, "MAGENTA");
        add(window, brick);
    }
    
    for (int i = 0; i < COLS; i++)
    {
        GRect brick = newGRect(((WIDTH-60)/COLS) * i + 30, 155, (WIDTH-60)/COLS, 15);
        setFilled(brick, true);
        setColor(brick, "ORANGE");
        add(window, brick);
    }
    
    for (int i = 0; i < COLS; i++)
    {
        GRect brick = newGRect(((WIDTH-60)/COLS) * i + 30, 170, (WIDTH-60)/COLS, 15);
        setFilled(brick, true);
        setColor(brick, "YELLOW");
        add(window, brick);
    }
    
    for (int i = 0; i < COLS; i++)
    {
        GRect brick = newGRect(((WIDTH-60)/COLS) * i + 30, 185, (WIDTH-60)/COLS, 15);
        setFilled(brick, true);
        setColor(brick, "GREEN");
        add(window, brick);
    }
    
    for (int i = 0; i < COLS; i++)
    {
        GRect brick = newGRect(((WIDTH-60)/COLS) * i + 30, 200, (WIDTH-60)/COLS, 15);
        setFilled(brick, true);
        setColor(brick, "CYAN");
        add(window, brick);
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
