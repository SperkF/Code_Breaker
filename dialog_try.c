#include <gtk/gtk.h>
#include <libintl.h>

#define _(string) gettext (string)

struct myWidgets {
	GtkWidget *window;
	GtkWidget *label;
	GtkWidget *entry;
};

/* Callback function in which reacts to the "response" signal from the user in
 * the dialog window.
 */
static void
on_response (GtkDialog *dialog, gint response_id, gpointer user_data)
{
	struct myWidgets *mw = (struct myWidgets *) user_data;

	/* If the button clicked gives response OK (response_id being -5) */
	if (response_id == GTK_RESPONSE_OK)
		gtk_label_set_text (GTK_LABEL (mw->label),
				    gtk_entry_get_text (GTK_ENTRY (mw->entry)));
	/* If the button clicked gives response CANCEL (response_id being -6) */
	else if (response_id == GTK_RESPONSE_CANCEL)
		gtk_label_set_text (GTK_LABEL (mw->label), "Cancel clicked!");
	/* If the message dialog is destroyed (for example by pressing escape) */
	else if (response_id == GTK_RESPONSE_DELETE_EVENT)
		gtk_label_set_text (GTK_LABEL (mw->label), "ESC pressed!");

  else if (response_id == GTK_RESPONSE_YES)
  		gtk_label_set_text (GTK_LABEL (mw->label), "YES BUTTON clicked!");

      else if (response_id == 666)
          gtk_label_set_text (GTK_LABEL (mw->label), "The number of the beast!");

	/* Destroy the dialog after one of the above actions have taken place */
	gtk_widget_destroy (GTK_WIDGET (dialog));
}

/*
 * Custom Dialog Window
 */
static void
dialog_cb (GtkWidget *widget, gpointer user_data)
{
	GtkWidget *dialog;
	GtkWidget *grid;
	GtkWidget *label;
	GtkWidget *content_area;
	struct myWidgets *mw = (struct myWidgets *) user_data;

	/* Create a new dialog, and set the parameters as follows:
	 * Dialog Flags - make the constructed dialog modal
	 * (modal windows prevent interaction with other windows in the application)
	 * Buttons Type - use the ok and cancel buttons
	 */
	dialog = gtk_dialog_new_with_buttons ("A Custom Dialog",
					      GTK_WINDOW (mw->window),
					      GTK_DIALOG_MODAL,
					      _ ("_Cancel"),
					      GTK_RESPONSE_CANCEL,
					      _ ("_Save Stats"),
					      GTK_RESPONSE_OK,
                _ ("Yes button"),
                GTK_RESPONSE_YES,
                _ ("NO button"),
                666,
					      NULL);
	/* Create a custom dialog below using a label, a text entry and a separator
	 * and attach it to the content area of the dialog.
	 */
	content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
	grid = gtk_grid_new();
	gtk_grid_set_row_homogeneous (GTK_GRID (grid), TRUE);
	gtk_grid_set_column_homogeneous (GTK_GRID (grid), TRUE);
	gtk_grid_set_row_spacing (GTK_GRID (grid), 10);
	gtk_container_add (GTK_CONTAINER (content_area), grid);
	gtk_container_set_border_width (GTK_CONTAINER (content_area), 10);
	label = gtk_label_new ("Enter some text below and click OK ...");
	gtk_grid_attach (GTK_GRID (grid), label, 0, 0, 1, 1);
	mw->entry = gtk_entry_new();
	gtk_grid_attach (GTK_GRID (grid), mw->entry, 0, 1, 1, 1);
	/* add some space below the content area and above the buttons */
	gtk_widget_set_margin_bottom(grid, 20);
	/* display the message dialog */
	gtk_widget_show_all (dialog);
	/* connect the response signal */
	g_signal_connect (GTK_DIALOG (dialog), "response",
			  G_CALLBACK (on_response), mw);
}

static void
activate (GtkApplication *app, gpointer user_data)
{
	GtkWidget *grid;
	GtkWidget *button;
	struct myWidgets *mw = (struct myWidgets *) user_data;

	/* Create a window with a title and a default size */
	mw->window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (mw->window), "Custom Dialog Example");
	gtk_window_set_default_icon_from_file ("icon.png", NULL);
	/* add a label and a button to the window */
	grid = gtk_grid_new();
	gtk_container_add (GTK_CONTAINER (mw->window), grid);
	gtk_container_set_border_width (GTK_CONTAINER (mw->window), 70);
	gtk_grid_set_row_homogeneous (GTK_GRID (grid), TRUE);
	gtk_grid_set_column_homogeneous (GTK_GRID (grid), TRUE);
	mw->label = gtk_label_new ("Click the button");
	gtk_widget_set_size_request (mw->label, 150, 40);
	gtk_grid_attach (GTK_GRID (grid), mw->label, 0, 0, 1, 1);
	button = gtk_button_new_with_label ("Show Dialog");
	gtk_widget_set_size_request (button, 150, 40);
	gtk_grid_attach (GTK_GRID (grid), button, 0, 1, 1, 1);
	/* connect a callback to the button click */
	g_signal_connect (button, "clicked", G_CALLBACK (dialog_cb), mw);
	/* show the window */
	gtk_widget_show_all (mw->window);
}

int
main (int argc, char **argv)
{
	GtkApplication *app;
	int status;
	struct myWidgets *mw = g_malloc (sizeof (struct myWidgets));

	app = gtk_application_new ("org.gtk.customdialog", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), mw);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);
	g_free (mw);

	return status;
}
/** EOF */
