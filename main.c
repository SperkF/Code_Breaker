#include <gtk/gtk.h>
#include <string.h>
#include "functions.h"
#include "dataTypes.h"

// map menu actions to callbacks
const GActionEntry app_actions[] = {
	{ "submit", submit_button_callback, NULL, NULL, NULL, {0, 0, 0} },
	{ "retry", retry_button_callback, NULL, NULL, NULL, {0, 0, 0} },
	{ "alph_easy", alph_easy_callback, NULL, NULL, NULL, {0, 0, 0} },
	{ "alph_med", alph_med_callback, NULL, NULL, NULL, {0, 0, 0} },
	{ "alph_hard", alph_hard_callback, NULL, NULL, NULL, {0, 0, 0} },
	{ "tries_easy", tries_easy_callback, NULL, NULL, NULL, {0, 0, 0} },
	{ "tries_med", tries_med_callback, NULL, NULL, NULL, {0, 0, 0} },
	{ "tries_hard", tries_hard_callback, NULL, NULL, NULL, {0, 0, 0} },
	{ "about", about_callback, NULL, NULL, NULL, {0, 0, 0} },
	{ "stats", stats_callback, NULL, NULL, NULL, {0, 0, 0} }
};
/****************************************************** FILE LOCAL PROTOTYPES */
static void construct_menu (GtkApplication *app, gpointer data);

static void
startup (GApplication *app, gpointer data)
{
	struct my_widgets *a = (struct my_widgets *) data;
	// connect actions with callbacks
	g_action_map_add_action_entries (G_ACTION_MAP (app), app_actions,
					 G_N_ELEMENTS (app_actions), (gpointer) a);
}




/*********************************************************************** Menu */
static void
construct_menu (GtkApplication *app, gpointer data)
{
	(void)app; //app is not used in function
	//typecast of passed on parameter (from gpointer back to meaninfull type)
	struct my_widgets *a = (struct my_widgets *) data;

	//widget to reffernce menu bar
	GtkWidget *headerbar;
	//buttons inside headerbar
	GtkWidget *game_info_button;
	// the menu displayed as a popover below the game_info_button
	GMenu *tries_difficulty;
	GMenu *alph_difficulty;
	GMenu *about_text;


	// create a headerbar
	headerbar = gtk_header_bar_new ();
	gtk_widget_show (headerbar);
	gtk_header_bar_set_title (GTK_HEADER_BAR (headerbar), "Codebreaker");
	gtk_header_bar_set_subtitle (GTK_HEADER_BAR (headerbar), "by Fabian Sperk");
	gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (headerbar), TRUE);
	gtk_window_set_titlebar (GTK_WINDOW (a->window), headerbar);

	// create an SUBMIT button, add it to the headerbar and connect the callbacks
	a->submit_button = gtk_button_new_with_label ("Submit");
	gtk_header_bar_pack_start (GTK_HEADER_BAR (headerbar), a->submit_button);
	gtk_actionable_set_action_name (GTK_ACTIONABLE (a->submit_button), "app.submit");
	/*further signal connections -> to reset counter inside those functions*/
	g_signal_connect (a->submit_button, "clicked", G_CALLBACK (cb_code1_clicked_change_apperance), NULL);
	g_signal_connect (a->submit_button, "clicked", G_CALLBACK (cb_code2_clicked_change_apperance), NULL);
	g_signal_connect (a->submit_button, "clicked", G_CALLBACK (cb_code3_clicked_change_apperance), NULL);
	g_signal_connect (a->submit_button, "clicked", G_CALLBACK (cb_code4_clicked_change_apperance), NULL);

	// create an RTRY button, add it to the headerbar and connect the callbacks
	a->retry_button = gtk_button_new_with_label ("Retry");
	gtk_header_bar_pack_start (GTK_HEADER_BAR (headerbar), a->retry_button);
	gtk_actionable_set_action_name (GTK_ACTIONABLE (a->retry_button), "app.retry");
	g_signal_connect (a->retry_button, "clicked", G_CALLBACK (cb_code1_clicked_change_apperance), NULL);
	g_signal_connect (a->retry_button, "clicked", G_CALLBACK (cb_code2_clicked_change_apperance), NULL);
	g_signal_connect (a->retry_button, "clicked", G_CALLBACK (cb_code3_clicked_change_apperance), NULL);
	g_signal_connect (a->retry_button, "clicked", G_CALLBACK (cb_code4_clicked_change_apperance), NULL);

	// create the GAME_INFO menu button ->when this button is pressed submenu opens
	game_info_button = gtk_menu_button_new();
	gtk_header_bar_pack_end (GTK_HEADER_BAR (headerbar), game_info_button);
	//submenu for game_info_button
	tries_difficulty = g_menu_new();
	g_menu_append (tries_difficulty, "tries easy", "app.tries_easy");
	g_menu_append (tries_difficulty, "tries medium", "app.tries_med");
	g_menu_append (tries_difficulty, "tries hard", "app.tries_hard");
	alph_difficulty = g_menu_new();
	g_menu_append (alph_difficulty, "code easy", "app.alph_easy");
	g_menu_append (alph_difficulty, "code medium", "app.alph_med");
	g_menu_append (alph_difficulty, "code hard", "app.alph_hard");
	//in order to seperate tries_difficulty from the alph_difficulty by a line ->g_menu_append_section
	g_menu_append_section (tries_difficulty, NULL, G_MENU_MODEL (alph_difficulty));
	about_text = g_menu_new();
	g_menu_append (about_text, "about", "app.about");
	g_menu_append (about_text, "statistics", "app.stats");
	//in order to seperate tries_difficulty from the alph_difficulty by a line ->g_menu_append_section
	g_menu_append_section (tries_difficulty, NULL, G_MENU_MODEL (about_text));

	//verbinde den game_info_button mit dem erstellten menü tries_difficulty und all dessen sub-menüs
	gtk_menu_button_set_menu_model (GTK_MENU_BUTTON (game_info_button),	G_MENU_MODEL (tries_difficulty));

	/* Statusbar */
	a->statusbar = gtk_statusbar_new();
	gtk_grid_attach (GTK_GRID (a->grid), a->statusbar, 1, (9 +  2*(a->current_tries_difficulty)), 7, 1);
	a->id = gtk_statusbar_get_context_id (GTK_STATUSBAR (a->statusbar), "demo");

	/*setup timer to call TimerCallback every 1000ms*/
	g_timeout_add (1000, TimerCallback, (gpointer) a);



	g_object_unref (alph_difficulty);
	g_object_unref (tries_difficulty);
	g_object_unref (about_text);

}



// app activate callback - creates the window
static void activate (GtkApplication* app, gpointer user_data)
{
	(void)app;
	struct my_widgets *s_widget = (struct my_widgets*) user_data;

	/*add CSS functionality*/
	GtkCssProvider *cssProvider;
	cssProvider = gtk_css_provider_new();
	gtk_css_provider_load_from_path (cssProvider, "bdemo.css", NULL);
	gtk_style_context_add_provider_for_screen (gdk_screen_get_default(),
						   GTK_STYLE_PROVIDER (cssProvider),
						   GTK_STYLE_PROVIDER_PRIORITY_USER);


	/* create the window and associate a title and an icon */
	s_widget->window = gtk_application_window_new (s_widget->app);
	gtk_window_set_title (GTK_WINDOW (s_widget->window), "Welcome");
	//gtk_window_set_default_icon_from_file ("icon.ppm", NULL);

	/*  set window properties */
	// remove the resizeable property from the window
	gtk_window_set_resizable (GTK_WINDOW (s_widget->window), FALSE);
	// start the window in the center of the screen
	gtk_window_set_position (GTK_WINDOW (s_widget->window), GTK_WIN_POS_CENTER);


	/* create a grid to be used as layout container */
	s_widget->grid = gtk_grid_new();
	// make the grid homogenous
//	gtk_grid_set_column_homogeneous (GTK_GRID (s_widget->grid), TRUE);
	// add some spacing between the vertical and horizontal grid
	gtk_grid_set_row_spacing (GTK_GRID (s_widget->grid), 5);
	gtk_grid_set_column_spacing (GTK_GRID (s_widget->grid), 5);
	// add some spacing on the bortder of the grid
	gtk_widget_set_margin_start (s_widget->grid, 5);
	gtk_widget_set_margin_end (s_widget->grid, 5);
	gtk_widget_set_margin_top (s_widget->grid, 5);
	gtk_widget_set_margin_bottom (s_widget->grid, 5);
	// add the grid to the window
	gtk_container_add (GTK_CONTAINER (s_widget->window), s_widget->grid);


	/*  fill grid with content */
	code_breaker_add_new_row (s_widget->grid, 1, & (s_widget->row1), 0, s_widget);
	code_breaker_add_new_row (s_widget->grid, 2, & (s_widget->row2), GREY_OUT, s_widget);
	code_breaker_add_new_row (s_widget->grid, 3, & (s_widget->row3), GREY_OUT, s_widget);
	code_breaker_add_new_row (s_widget->grid, 4, & (s_widget->row4), GREY_OUT, s_widget);
	code_breaker_add_new_row (s_widget->grid, 5, & (s_widget->row5), GREY_OUT, s_widget);
	code_breaker_add_new_row (s_widget->grid, 6, & (s_widget->row6), GREY_OUT, s_widget);
	code_breaker_add_new_row (s_widget->grid, 7, & (s_widget->row7), GREY_OUT, s_widget);
	code_breaker_add_new_row (s_widget->grid, 8, & (s_widget->row8), GREY_OUT, s_widget);
	code_breaker_add_new_row (s_widget->grid, 9, & (s_widget->row9), GREY_OUT, s_widget);
	code_breaker_add_new_row (s_widget->grid, 10, & (s_widget->row10), GREY_OUT, s_widget);
	code_breaker_add_new_row (s_widget->grid, 11, & (s_widget->row11), GREY_OUT, s_widget);
	code_breaker_add_new_row (s_widget->grid, 12, & (s_widget->row12), GREY_OUT, s_widget);


 /* moved the creation of the menu to a dedicated function */
 construct_menu (s_widget->app, (gpointer) s_widget);

	/*	display everything	*/
	gtk_widget_show_all (s_widget->window);

	/*********************************************************** STARTUP CALLBACK */
}

int main (int argc, char **argv)
{
	int status;
	// we need some memory for the widgets struct
	struct my_widgets *s_widget;
	if( (s_widget = g_malloc (sizeof (struct my_widgets))) == NULL)
	{
		fprintf (stderr, "ERROR: (file: %s | line: %d) g_malloc of game_statistics.txt failed\n", __FILE__, __LINE__);
		exit (EXIT_FAILURE);
	}
	s_widget->code[0] = '0';
	s_widget->code[1] = '1';
	s_widget->code[2] = '2';
	s_widget->code[3] = '3';
	s_widget->code[4] = '4';
	s_widget->code[5] = '5';
	s_widget->code[6] = '6';
	s_widget->code[7] = '7';
	//set game settings to default
	s_widget->alph_difficulty = 0;
	s_widget->prev_tries_difficulty = 2;
	s_widget->current_tries_difficulty = 2;
	s_widget->gametime = 0;
	randomize(s_widget->code, 4+((s_widget->alph_difficulty)*2) );
	#if FS_DEBUG
		for(int i = 0; i < 4+((s_widget->alph_difficulty)*2); i++)
			printf("code: %c\n",s_widget->code[i]);
		printf("--------------------------");
	#endif
	// create a threaded application
	s_widget->app = gtk_application_new (0, G_APPLICATION_FLAGS_NONE);
	g_signal_connect (s_widget->app, "activate", G_CALLBACK (activate), (gpointer) s_widget);
	g_signal_connect (s_widget->app, "startup", G_CALLBACK (startup), (gpointer) s_widget);
	// run the application -> emits an "activate" signal
	status = g_application_run (G_APPLICATION (s_widget->app), argc, argv);
	g_object_unref (s_widget->app);

	// free the memory for the widgets struct
	g_free (s_widget);
	return status;

}
/** EOF */
