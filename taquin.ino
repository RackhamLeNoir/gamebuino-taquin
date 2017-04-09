#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;

extern const byte font3x5[];
extern const byte font5x7[];

uint8_t values[16];
uint8_t emptypos = 0;
unsigned int moves = 0;

#define MAXSHUFFLES 200

#define UP    0
#define DOWN  1
#define LEFT  2
#define RIGHT 3

#define CELLHEIGHT 12
#define CELLWIDTH 15

void makemove(uint8_t direction)
{
  switch(direction)
  {
    case UP:
      if (emptypos < 12)
      {
        values[emptypos] = values[emptypos + 4];
        emptypos += 4;
      }
      break;
    case DOWN:
      if (emptypos > 3)
      {
        values[emptypos] = values[emptypos - 4];
        emptypos -= 4;
      }
      break;
    case LEFT:
      if (emptypos % 4 != 3)
      {
        values[emptypos] = values[emptypos + 1];
        emptypos += 1;
      }
      break;
    case RIGHT:
      if (emptypos % 4 != 0)
      {
        values[emptypos] = values[emptypos - 1];
        emptypos -= 1;
      }
      break;
  }
}

void init_game()
{
  int i;
  for (i = 0 ; i < 16 ; i++)
    values[i] = i;
  emptypos = 15;
  moves = 0;

  //shuffle
  for (i = 0 ; i < MAXSHUFFLES ; i++)
    makemove((uint8_t)random(4));
}

bool gamewon()
{
  int i;
  for (i = 0 ; i < 15 ; i++)
    if (values[i] != i)
      return false;
  return true;
}

int numlength(int number)
{
  if (number == 0)
    return 1;
  else if (number > 0)
    return floor(log10(number)) + 1;
  return floor(log10(number)) + 2;
}

void setup()
{
  gb.begin();
  gb.titleScreen(F("Taquin"));
  gb.pickRandomSeed();
  gb.battery.show = true;

  init_game();
}

void drawgame()
{
  int i, j;
  gb.display.setFont(font5x7);
  for (i = 0 ; i < 4 ; i++)
  {
    for (j = 0 ; j < 4 ; j++)
    {
      if (4 * i + j != emptypos)
      {
        if (values[4 * i + j] < 9)
          gb.display.cursorX = j * CELLWIDTH + (CELLWIDTH - 5) / 2;
        else
          gb.display.cursorX = j * CELLWIDTH+ (CELLWIDTH- 11) / 2;
        gb.display.cursorY = i * CELLHEIGHT+ (CELLHEIGHT - 7) / 2 + 1;
        gb.display.print(values[4 * i + j] + 1);
      }
      gb.display.drawRect(j * CELLWIDTH, i * CELLHEIGHT, CELLWIDTH, CELLHEIGHT);
    }
  }
  gb.display.setFont(font3x5);
  gb.display.cursorX =  4 * CELLWIDTH + 1;
  gb.display.cursorY = 5;
  gb.display.println(moves);
  gb.display.cursorX =  4 * CELLWIDTH + 1;
  gb.display.print("moves");
}

void drawwin()
{
  gb.display.setFont(font5x7);
  gb.display.cursorX = (LCDWIDTH - 8 * 6) / 2;
  gb.display.cursorY = (LCDHEIGHT - 5 * 8) / 2;
  gb.display.println("You WON!");
  gb.display.cursorX = (LCDWIDTH - (3 + numlength(moves)) * 6) / 2;
  gb.display.print("in ");
  gb.display.println(moves);
  gb.display.cursorX = (LCDWIDTH - 5 * 6) / 2;
  gb.display.println("moves");
  gb.display.println("");
  gb.display.setFont(font3x5);
  gb.display.cursorX = (LCDWIDTH - 18 * 4) / 2;
  gb.display.print("Press \25 to restart");
}

void inputsgame()
{
  if(gb.buttons.pressed(BTN_C))
    gb.titleScreen(F("Taquin"));

  if (gb.buttons.pressed(BTN_UP))
  {
    makemove(UP);
    moves++;
  } 
  else if (gb.buttons.pressed(BTN_DOWN))
  {
    makemove(DOWN);
    moves++;
  } 
  if (gb.buttons.pressed(BTN_LEFT))
  {
    makemove(LEFT);
    moves++;
  } 
  else if (gb.buttons.pressed(BTN_RIGHT))
  {
    makemove(RIGHT);
    moves++;
  } 
}

void inputswin()
{
  if(gb.buttons.pressed(BTN_A))
    init_game();
  if(gb.buttons.pressed(BTN_C))
    gb.titleScreen(F("Taquin"));
}

void loop(){
  if(gb.update())
  {
    if (gamewon())
    {
      inputswin();
      drawwin();
    }
    else
    {
      inputsgame();
      drawgame();
    }
  }
}

