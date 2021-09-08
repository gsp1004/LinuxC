#include<stdio.h>
#include<stdlib.h>
#include<termios.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>


typedef enum color_e
{
	COLOR_FG_BLACK = 30,
	COLOR_FG_RED   ,
	COLOR_FG_GREEN ,
	COLOR_FG_BROWN ,
	COLOR_FG_BLUE  ,
	COLOR_FG_PINK  ,
	COLOR_FG_BLUE2 ,
	COLOR_FG_WHITE ,
	COLOR_BG_BLACK = 40,
	COLOR_BG_RED   ,
	COLOR_BG_GREEN ,
	COLOR_BG_BROWN ,
	COLOR_BG_BLUE  ,
	COLOR_BG_PINK  ,
	COLOR_BG_BLUE2 ,
	COLOR_BG_WHITE ,
}color;

//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
static struct termios old_env;

void termios_init(void)
{
	struct termios new;
	
	tcgetattr(0, &old_env);
	tcgetattr(0, &new);
	new.c_lflag &= ~(ICANON | ECHO);
	new.c_cc[VMIN] = 1;
	new.c_cc[VTIME] = 0;
	
	tcsetattr(0, TCSANOW, &new);
	printf("\033[2J");    // clear image
	printf("\033[?25l");  // hide cursor
	return;
}

void termios_restore(void)
{
	tcsetattr(0, TCSANOW, &old_env);
	return;
}
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void button_up()
{
	printf("%s\n",__FUNCTION__);
}

void button_down()
{
	printf("%s\n",__FUNCTION__);
}

void button_right()
{
	printf("%s\n",__FUNCTION__);
}

void button_left()
{
	printf("%s\n",__FUNCTION__);
}

#if 1
#define  left_space (16)
#define  top_space (9)
#define  play_area_w (10)
#define  play_area_h (21)
#define  game_box_line_width (1)

#define  game_box_w (play_area_w + 2)
#define  game_box_h (play_area_h + 2)
#define  game_box_x (left_space + game_box_line_width)
#define  game_box_y (top_space + game_box_line_width)

#define  mid_space (3)

#define right_space (left_space + game_box_line_width*2 + play_area_w + mid_space)

#define  next_box_w (4 + 2)
#define  next_box_h (4 + 2)
#define  next_box_line_width (1)  // (game_box_line_width)
#define  next_box_x (right_space + next_box_line_width)
#define  next_box_y (top_space + next_box_line_width + 1)

#define  next_str_start_x (right_space + next_box_w/2 + next_box_line_width - 1)
#define  next_str_start_y (next_box_y - next_box_line_width)


#define  level_str_start_x (next_str_start_x)
#define  level_str_start_y (next_str_start_y + next_box_h + next_box_line_width*2)

#define  level_num_start_x (level_str_start_x)
#define  level_num_start_y (level_str_start_y + 1)


#define  score_str_start_x (next_str_start_x)
#define  score_str_start_y (level_num_start_y + 3)

#define  score_num_start_x (score_str_start_x)
#define  score_num_start_y (score_str_start_y + 1)

int score = 0;
int level = 990;

#endif

/* first, we need to design the game layout:
 * 3 pixel left and top space(6 cursor). 
 * 1 pixel frame left and top
 * 13*26 pixel play area
 * 1 pixel frame right and buttom
 * 3 pixel right space(6 cursor). 
*/



void fill_position(char *cmd, int x, int y)
{
	char tmp[32] = {0};
	sprintf(tmp,"\033[%d;%dH",x,y);
	strcat(cmd,tmp);
}


/*
 * fill_color - fill the color attr to the cmd.
 *              if do not want to set one attr, use 0
*/
void fill_color( char *cmd, int fg, int bg)
{
	char tmp[32] = {0};
	if(0 == fg && 0 == bg)
		return;

	if(fg && bg){
		sprintf(tmp,"\033[%d;%dm",fg,bg);
		strcat(cmd,tmp);
	}else{
		sprintf(tmp,"\033[%dm",fg?fg:bg);
		strcat(cmd,tmp);
	}
}

void fill_str(char *cmd, char *str)
{
	strcat(cmd,str);
	strcat(cmd,"\033[0m\n");
}










/*
 * draw_pixel - draw a pixel in one place
 * @x: x coordinate, horizontal coordinate
 * @y: y coordinate, vertical coordinate
 * @color: the color you wanna full with
 * @lc: the left char to full
 * @rc: the right char to full
*/
void draw_pixel(const int x, const int y, color c, const char lc, const char rc)
{
	if(c<COLOR_FG_BLACK || c>COLOR_BG_WHITE){
		fprintf(stderr,"unkonwn color %d, use default fg_black!\n",c);
		c = COLOR_FG_BLACK;
	}
	printf("\033[%d;%dH\033[31;%dm%c\033[0m\n",y,x*2-1,c,lc);
	printf("\033[%d;%dH\033[31;%dm%c\033[0m\n",y,x*2,c,rc);
}

/*
 * 
*/
void show_str(int x, int y, char *s, int fg, int bg)
{
	printf("\033[%d;%dH\033[%d;%dm%s\033[0m\n",y, x*2-1, fg, bg, s);
}

void draw_box_pixel(int x, int y)
{
	draw_pixel(x,y,COLOR_BG_BLUE2,' ',' ');
}


/**
 * draw_box -- draw the 
*/
void draw_box(int x, int y, int w, int h, int line_w)
{
	int i,j;
	for(i=x-line_w+1;i<x+w+line_w-1;i++)  // draw top horizontal line
		for(j=y;j>y-line_w;j--)
			draw_box_pixel(i,j);

	for(i=x-line_w+1;i<x+w+line_w-1;i++)  // draw bottom horizontal line
		for(j=y+h-1;j<y+h-1+line_w;j++)
			draw_box_pixel(i,j);

	for(i=x;i>x-line_w;i--)
		for(j=y;j<y+h;j++)
			draw_box_pixel(i,j);

	for(i=x+w-1;i<x+w-1+line_w;i++)
		for(j=y;j<y+h;j++)
			draw_box_pixel(i,j);

}


void draw_box_game(void)
{
	draw_box(game_box_x, game_box_y, game_box_w, game_box_h, game_box_line_width);
}

void draw_box_next(void)
{
	draw_box(next_box_x, next_box_y, next_box_w, next_box_h, next_box_line_width);
	show_str(next_str_start_x, next_str_start_y, "next", COLOR_FG_RED, COLOR_BG_BLUE);
}


void show_level(int l)
{
	// has only 9 level now
	char s[] = "     ";
	if(l<10){
		s[2] = '0' + l;
	}
	else if(10<=l && l<100){
		s[2] = '0' + l/10;
		s[3] = '0' + l%10;
	}
	else if(100<=l && l<1000){
		s[1] = '0' + l/100;
		s[2] = '0' + (l%100)/10;
		s[3] = '0' + l%10;
	}
	show_str(level_str_start_x, level_str_start_y, "level", COLOR_FG_RED, COLOR_BG_BLUE);
	//show_str(level_num_start_x, level_num_start_y, "  0  ", COLOR_FG_RED, COLOR_BG_BLUE);
	show_str(level_num_start_x, level_num_start_y, s, COLOR_FG_RED, COLOR_BG_BLUE);
}

void show_score(int num)
{
	char s[16] = {0};
	sprintf(s,"%5d",num);
	
	show_str(score_str_start_x, score_str_start_y, "score", COLOR_FG_RED, COLOR_BG_BLUE);
	show_str(score_num_start_x, score_num_start_y, s, COLOR_FG_RED, COLOR_BG_BLUE);
}

/*
 * 
*/
void draw(void)
{
	//printf("\033[2J");    // clear image
	draw_box_game();
	draw_box_next();
	show_level(level);
	show_score(score);


}


void * fresh_image(void * nouse)
{
	while(1){
		usleep(33 *1000);
		draw();
		score+=99999;
		level = (1+level)%1000;
	}
}

int main()
{
	pthread_t p;
	termios_init();
	pthread_create(&p, NULL, fresh_image, NULL);
	draw();

#if 1  // use termios to modify input device
	int ch;
	while(1){
		ch = getchar();

		switch(ch){
			case 0x1b:
				ch = getchar();
				if(0x5b != ch) break;  // not up/down/right/left

				ch = getchar();
				if(0x41 == ch) button_up();
				else if(0x42 == ch) button_down();
				else if(0x43 == ch) button_right();
				else if(0x44 == ch) button_left();
				break;
			case 'w':
				button_up();
				break;
			case 's':
				button_down();
				break;
			case 'a':
				button_left();
				break;
			case 'd':
				button_right();
				break;
			default:
				break;
		}
	}
	
#endif
	termios_restore();
	exit(1);
}
