/*!
 * Simple GTK Greeter Demo Application
 *
 * Version 3 - beautifying
 *             - make the window of fixed size, associate an icon, place the
 *               window upon start in the center of the screen
 *             - adds some spacing between and around the grid
 *             - justify the name label
 *             - adds a placeholder to the entry box
 *             - sets default sizes to some widgets
 *
 * M. Horauer
 *
 * Build instructions:
 * gcc hello_gtk_v3.c `pkg-config --cflags --libs gtk+-3.0` -o welcome -Wall -g
 */
#include <gtk/gtk.h>
#include <string.h>
#include "functions.h"
#include "dataTypes.h"





// app activate callback - creates the window
static void activate (GtkApplication* app, gpointer user_data)
{
	struct my_widgets *s_widget = (struct my_widgets*) user_data;

	GtkWidget *window;
	GtkWidget *grid;
	/*add CSS functionality*/
	GtkCssProvider *cssProvider;
	cssProvider = gtk_css_provider_new();
	gtk_css_provider_load_from_path (cssProvider, "bdemo.css", NULL);
	gtk_style_context_add_provider_for_screen (gdk_screen_get_default(),
						   GTK_STYLE_PROVIDER (cssProvider),
						   GTK_STYLE_PROVIDER_PRIORITY_USER);


	/* create the window and associate a title and an icon */
	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "Welcome");
	gtk_window_set_default_icon_from_file ("icon.ppm", NULL); //icon..ppm must be in same dir as exe

	/*  set window properties */
	// remove the resizeable property from the window
	gtk_window_set_resizable (GTK_WINDOW (window), FALSE);
	// start the window in the center of the screen
	gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);


	/* create a grid to be used as layout container */
	grid = gtk_grid_new();
	// make the grid homogenous
	gtk_grid_set_column_homogeneous (GTK_GRID (grid), TRUE);
	// add some spacing between the vertical and horizontal grid
	gtk_grid_set_row_spacing (GTK_GRID (grid), 5);
	gtk_grid_set_column_spacing (GTK_GRID (grid), 5);
	// add some spacing on the bortder of the grid
	gtk_widget_set_margin_start (grid, 5);
	gtk_widget_set_margin_end (grid, 5);
	gtk_widget_set_margin_top (grid, 5);
	gtk_widget_set_margin_bottom (grid, 5);
	// add the grid to the window
	gtk_container_add (GTK_CONTAINER (window), grid);


	/*  create new widgets and set paramters for them*/
	//new widget: test button
	s_widget->button_test = gtk_button_new_with_label ("test");
	gtk_widget_set_name (s_widget->button_test, "button_state0");
	gtk_widget_set_size_request (s_widget->button_test, 30, 30);
	g_signal_connect (s_widget->button_test, "clicked", G_CALLBACK (test_button_clicked), (gpointer) s_widget);
	//further cb functioons that we want to envoke if test button is clicked ->will reset their counter
	g_signal_connect (s_widget->button_test, "clicked", G_CALLBACK (cb_code1_clicked_change_apperance), (gpointer) s_widget);
		g_signal_connect (s_widget->button_test, "clicked", G_CALLBACK (cb_code2_clicked_change_apperance), (gpointer) s_widget);
			g_signal_connect (s_widget->button_test, "clicked", G_CALLBACK (cb_code3_clicked_change_apperance), (gpointer) s_widget);
				g_signal_connect (s_widget->button_test, "clicked", G_CALLBACK (cb_code4_clicked_change_apperance), (gpointer) s_widget);
	/*  fill grid with content */
	gtk_grid_attach (GTK_GRID (grid), s_widget->button_test, 0, 0, 2, 1);



	code_breaker_add_new_row (grid, 1, & (s_widget->row1), 0);
	code_breaker_add_new_row (grid, 2, & (s_widget->row2), GREY_OUT);
	code_breaker_add_new_row (grid, 3, & (s_widget->row3), GREY_OUT);
	code_breaker_add_new_row (grid, 4, & (s_widget->row4), GREY_OUT);
	code_breaker_add_new_row (grid, 5, & (s_widget->row5), GREY_OUT);
	code_breaker_add_new_row (grid, 6, & (s_widget->row6), GREY_OUT);
	code_breaker_add_new_row (grid, 7, & (s_widget->row7), GREY_OUT);
	code_breaker_add_new_row (grid, 8, & (s_widget->row8), GREY_OUT);
	code_breaker_add_new_row (grid, 9, & (s_widget->row9), GREY_OUT);
	code_breaker_add_new_row (grid, 10, & (s_widget->row10), GREY_OUT);
	code_breaker_add_new_row (grid, 11, & (s_widget->row11), GREY_OUT);
	code_breaker_add_new_row (grid, 12, & (s_widget->row12), GREY_OUT);

//run trough lines

	/*	display everything	*/
	gtk_widget_show_all (window);
}

int main (int argc, char **argv)
{
	GtkApplication *app;
	int status;
	// we need some memory for the widgets struct
	struct my_widgets *s_widget = g_malloc (sizeof (struct my_widgets));
	// create a threaded application
	app = gtk_application_new (NULL, G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), (gpointer) s_widget);
	// run the application -> emits an "activate" signal
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);

	// free the memory for the widgets struct
	g_free (s_widget);

	return status;
}
/** EOF */
