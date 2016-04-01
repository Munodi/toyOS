#ifdef x86

#include <toyOS/bootTerminal.hpp>
#include <toyOS/arch/x86/io.hpp>
#include <cstdint>
#include <cstdlib>

// The VGA framebuffer starts at 0xC00B8000.
static std::uint16_t *video_memory = (std::uint16_t*)0xC00B8000;
// Stores the cursor position.
static std::uint8_t cursor_x = 0;
static std::uint8_t cursor_y = 0;

// Updates the hardware cursor.
static void move_cursor()
{
  // The screen is 80 characters wide...
  std::uint16_t cursorLocation = cursor_y * 80 + cursor_x;
  out8(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
  out8(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
  out8(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
  out8(0x3D5, cursorLocation);      // Send the low cursor byte.
}

// Scrolls the text on the screen up by one line.
static void scroll()
{
  // Get a space character with the default colour attributes.
  std::uint8_t attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
  const std::uint16_t blank = 0x20 /* space */ | (attributeByte << 8);

  // Row 25 is the end, this means we need to scroll up
  if(cursor_y >= 25)
  {
    // Move the current text chunk that makes up the screen
    // back in the buffer by a line
    int i;
    for (i = 0*80; i < 24*80; i++)
      video_memory[i] = video_memory[i+80];

    // The last line should now be blank. Do this by writing
    // 80 spaces to it.
    for (i = 24*80; i < 25*80; i++)
      video_memory[i] = blank;

    // The cursor should now be on the last line.
    cursor_y = 24;
  }
}

// Writes a single character out to the screen.
void bootTerminal::printc(char c)
{
  // The background colour is black (0), the foreground is white (15).
  std::uint8_t backColour = 0;
  std::uint8_t foreColour = 15;

  // The attribute byte is made up of two nibbles - the lower being the
  // foreground colour, and the upper the background colour.
  std::uint8_t attributeByte = (backColour << 4) | (foreColour & 0x0F);
  // The attribute byte is the top 8 bits of the word we have to send to the
  // VGA board.
  std::uint16_t attribute = attributeByte << 8;

  // Handle a backspace, by moving the cursor back one space
  if (c == 0x08 && cursor_x)
    cursor_x--;

  // Handle a tab by increasing the cursor's X, but only to a point
  // where it is divisible by 8.
  else if (c == 0x09)
    cursor_x = (cursor_x+8) & ~(8-1);

  // Handle carriage return
  else if (c == '\r')
    cursor_x = 0;

  // Handle newline by moving cursor back to left and increasing the row
  else if (c == '\n')
  {
    cursor_x = 0;
    cursor_y++;
  }

  // Handle any other printable character.
  else if(c >= ' ')
  {
      video_memory[cursor_y*80 + cursor_x] = c | attribute;
      cursor_x++;
  }

  // Check if we need to insert a new line because we have reached the end
  // of the screen.
  if (cursor_x >= 80)
  {
      cursor_x = 0;
      cursor_y ++;
  }

  // Scroll the screen if needed.
  scroll();
  // Move the hardware cursor.
  move_cursor();
}

// Clears the screen, by copying lots of spaces to the framebuffer.
void bootTerminal::cls()
{
  // Make an attribute byte for the default colours
  std::uint8_t attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
  std::uint16_t blank = 0x20 /* space */ | (attributeByte << 8);

  int i;
  for (i = 0; i < 80*25; i++)
    video_memory[i] = blank;

  // Move the hardware cursor back to the start.
  cursor_x = 0;
  cursor_y = 0;
  move_cursor();
}

// Outputs a null-terminated ASCII string to the monitor.
void bootTerminal::prints(const char *c)
{
    while (*c)
      bootTerminal::printc(*c++);
}

void bootTerminal::printh(unsigned u)
{
    char tmp[32];
    bootTerminal::prints(std::utoa(u, tmp, 16));
}

void bootTerminal::print10(unsigned u)
{
    char tmp[32];
    bootTerminal::prints(std::utoa(u, tmp, 10));
}

void bootTerminal::init()
{
    bootTerminal::cls();
}

#endif // x86
