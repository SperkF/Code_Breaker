#ifndef _FSdataTypes_
#define _FSdataTypes_

/*
 * defines used in multiple files
 */
#define FS_DEBUG_BUTTON 0
#define ROWS_OF_GAME 12
#define GREY_OUT 420

/*
 * user defined dataTypes
 */
//struct that holds pointers to widgets of one row of the inner board (=excluse top and bottom menu)
struct widget_of_one_row {
	GtkWidget *label;
	GtkWidget *button_code1;
	GtkWidget *button_code2;
	GtkWidget *button_code3;
	GtkWidget *button_code4;
	GtkWidget *button_pos_hits;
	GtkWidget *button_col_hits;
};

// struct that collects all widgets we will use in various callbacks
struct my_widgets {
	/*mit array weiß ich nicht wie ich unten im progarm variablen von dem Typ weitergeben kann*/
	//struct widget_of_one_row s_inner_board_wigets[ROWS_OF_GAME];
	struct widget_of_one_row row1;
	struct widget_of_one_row row2;
	struct widget_of_one_row row3;
	struct widget_of_one_row row4;
	struct widget_of_one_row row5;
	struct widget_of_one_row row6;
	struct widget_of_one_row row7;
	struct widget_of_one_row row8;
	struct widget_of_one_row row9;
	struct widget_of_one_row row10;
	struct widget_of_one_row row11;
	struct widget_of_one_row row12;

	//struct widgets of top and bottom menu bar
	GtkWidget *button_test;
};

struct game_config {
	gchar difficulty;
};



/*
 * global variable used in multiple files
 */


#endif
/*EOF*/
