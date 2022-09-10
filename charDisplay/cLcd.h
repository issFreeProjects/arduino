/*
 *  initialize the lcd 
 *  must be culled at the beginning 
 * */
void init();

/*  send a command to lcd
 *  
 *  SRC: circuitdigest.com/article/16x2-lcd-display-module-pinout-datasheet
 *  Hex.      Command.
 *  0x0F      LCD ON, cursor ON
 *  0x01      Clear display screen
 *  0x02      Return home
 *  0x04      Decrement cursor (shift cursor to left)
 *  0x06      Increment cursor (shift cursor to right)
 *  0x05      Shift display right
 *  0x07      Shift display left
 *  0x0E      Display ON, cursor blinking
 *  0x80      Force cursor to beginning of first line
 *  0xC0      Force cursor to beginning of second line
 *  0x38      2 lines and 5×7 matrix
 *  0x8x      Cursor line 1 position x
 *  0x3C      Activate second line
 *  0x08      Display OFF, cursor OFF
 *  0xCx      Jump to second line, position x
 *  0xOC      Display ON, cursor OFF
 *
 * */
void lcd_command(uint8_t comm);

/*  send a char to lcd
 *  and show it at the end of the current line
 *  @c might be an ascci char or index of a custom symbol
 *
 *  Ex:  @c = 0x41 will print 'A'
 *       @c = 0    will print defined symbol at index=0,
 *                 if you've defined it via load_custom_sym function
 * */
void lcd_char(uint8_t c);

/*
 *    load @font as a custom font at @index possission
 *    loaded font is accessible via:  lcd_char(index)
 * */
void load_custom_sym(uint8_t *font, uint8_t index);

/*  
 *  write @str on the lcd
 *  at the end of the current line
 * */
void lcd_str(const char *str);

/*  move cursot to requested position
 *  if first 4 bits of @pos be 0 then move to first line
 *  otherwise move to the second line
 *
 *  Ex:
 *      @pos = 0x05  -->  move to line 1 and position 5
 *      @pos = 0xF6  -->  move to line 2 and position 6
 *
 * */
void lcd_goto(uint8_t pos);

/* clear and go to the beginning of the first line */
void lcd_clear_screen();
