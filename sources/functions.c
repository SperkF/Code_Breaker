// path/file ->to include files that are not in same directory
// ..=step out one directory; ->then step into includes directory
#include "../includes/functions.h"

#define DIALOG_RESPONSE_SHOW_STATS 10
#define DIALOG_RESPONSE_SAVE_STATS 66

//No lines are saved into file, No-1 lines are displayed upon request
#define MAX_LINES_INSIDE_STATUS_FILE 10

/* Callback function in which reacts to the "response" signal from the user in
 * the dialog window.
 */
void
on_response (GtkDialog *dialog, gint response_id, gpointer user_data)
{
	struct my_widgets *a = (struct my_widgets *) user_data;
	GtkWidget *popup_dialog;
	FILE *fp_game_stats;
	char *file_data;
	char *new_status_entry; //should be more than enough space to hold the string for new game stat line
	char ch=0;
	int line_cnt =0;
	int ch_cnt = 0;
	/*	check to see what caused response signal from game_won_dialog*/
		/* message dialog is destroyed (for example by pressing escape) */
	if(response_id == (gint)GTK_RESPONSE_DELETE_EVENT)
	{
#if FS_DEBUG
		printf ("game_won_dialog destroyed\n");
#endif
	}
	/* quit button clicked -> gives response CANCEL (response_id being -6) */
	if(response_id == (gint)GTK_RESPONSE_CANCEL)
	{
#if FS_DEBUG
		printf ("<cancel> Button in game_won_dialog pressed\n");
#endif
	}
	/* <save stats< or <show stats> button clicked -> gives response DIALOG_RESPONSE_SAVE_STATS (response_id being 666) */
	if(response_id == (gint)DIALOG_RESPONSE_SAVE_STATS || response_id ==  (gint)DIALOG_RESPONSE_SHOW_STATS)
	{
		/*****open file in binary read mode */
		fp_game_stats = fopen (TEXTFILE_GAME_STATS, "rb");
		if (fp_game_stats == NULL) {
			fprintf (stderr, "ERROR: (file: %s | line: %d) \
		 fopen() of game_statistics.txt failed\n", __FILE__, __LINE__);
			exit (EXIT_FAILURE);
		}
		/*****get file character count and line count (only for max defined lines)  */
		while( (ch = fgetc(fp_game_stats)) != EOF)
		{
			ch_cnt++;
			if(ch == '\n')
			{
				line_cnt++;
			}
			if(line_cnt == MAX_LINES_INSIDE_STATUS_FILE - 1)
			{
				break;
			}
		}
		rewind(fp_game_stats); //get file pointer back to first pos of file
		/*****allocate memory for buffer to hold file content*/
		file_data = calloc (ch_cnt+2, sizeof(char));
		if (file_data == NULL) {
			fclose (fp_game_stats);
			fprintf (stderr, "ERROR: (file: %s | line: %d) \
		 calloc() for file_data failed\n", __FILE__, __LINE__);
			exit (EXIT_FAILURE);
		}

		/*****read conten from file and save to buffer*/
		for(int i = 0; i < ch_cnt; i++)
		{
			*(file_data+i) = fgetc(fp_game_stats);
			if(*(file_data+i) == EOF)
			{
				fclose (fp_game_stats);
				free (file_data);
				fprintf (stderr, "ERROR: (file: %s | line: %d) \
			 unexpected behaviour while trying to read from game_stats file\n", __FILE__, __LINE__);
				exit (EXIT_FAILURE);
			}
		}

		/*****just be sure put an extra String terminator at the end*/
		file_data[ch_cnt + 1] = '\0';

		if (response_id == (gint)DIALOG_RESPONSE_SHOW_STATS) {
#if FS_DEBUG
			printf ("<show stats> Button in game_won_dialog pressed\n");
#endif
			/*	Popup that displays file content*/
			popup_dialog = gtk_message_dialog_new (GTK_WINDOW (dialog),
							 GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
							 GTK_MESSAGE_INFO,
							 GTK_BUTTONS_CLOSE,
							 "%s", (const gchar*) file_data);
			//		 connect popup with function -gtk_widget_destroy- from gtk library that will destroy it
			g_signal_connect (popup_dialog, "response", G_CALLBACK (gtk_widget_destroy), NULL);
			// display popup
			gtk_widget_show (popup_dialog);
			/*	close file-descriptor and free allocated memory space 	*/
			fclose (fp_game_stats);
			free (file_data);
			/*	return from cb without destroying the window or restarting the game		*/
			return;
		}

		if (response_id == (gint)DIALOG_RESPONSE_SAVE_STATS) {
#if FS_DEBUG
			printf ("<save stats> Button in game_won_dialog pressed\n");
#endif
		//allocate space for array to hold existing stats + new line
		//es wird angenommen das der neue Eintrag nicht mehr als 200 Zeichen benötigt
		new_status_entry = calloc (ch_cnt + 200, sizeof(char));
		if (new_status_entry == NULL) {
			free (file_data);
			fclose (fp_game_stats);
			fprintf (stderr, "ERROR: (file: %s | line: %d) \
		 calloc() for new_status_entry failed\n", __FILE__, __LINE__);
			exit (EXIT_FAILURE);
		}
		//reopen file in write mode (deletes current file content)
		/*fopen(writemode) ->If a file with the same name already exists, its contents are discarded
		and the file is treated as a new empty file.*/
		fp_game_stats = freopen (TEXTFILE_GAME_STATS, "w", fp_game_stats);
		if (fp_game_stats == NULL) {
			free (file_data);
			free (new_status_entry);
			fclose (fp_game_stats);
			fprintf (stderr, "ERROR: (file: %s | line: %d) \
		 fopen() of game_statistics.txt failed\n", __FILE__, __LINE__);
			exit (EXIT_FAILURE);
		}

		//grab name that user has enterd as well as seconds_it_took and current_row and put it together in a string
			if( (sprintf (new_status_entry,"PLAYER: %s won in %d SECONDS, it took him  till ROUND %d\n",\
			gtk_entry_get_text (GTK_ENTRY (a->entry)),a->seconds_it_took, a->current_row)) < 0)
			{
				free (file_data);
				free (new_status_entry);
				fclose (fp_game_stats);
				fprintf (stderr, "ERROR: (file: %s | line: %d) \
			 fsprintf() failed\n", __FILE__, __LINE__);
				exit (EXIT_FAILURE);
			}
		//new entry gets put in top; if MAX_LINES_INSIDE_STATS is reached, the bottom line should be deleted
		#if FS_DEBUG
			printf("%s",new_status_entry);
			#endif
			//strcat() new_status_entry + file_data ->new_status_entry
			if( (strcat(new_status_entry, file_data)) == NULL)
			{
				free (file_data);
				free (new_status_entry);
				fclose (fp_game_stats);
				fprintf (stderr, "ERROR: (file: %s | line: %d) \
			 strcat() failed\n", __FILE__, __LINE__);
				exit (EXIT_FAILURE);
			}
			if( (fprintf(fp_game_stats,"%s",new_status_entry)) < 0)
			{
				free (file_data);
				free (new_status_entry);
				fclose (fp_game_stats);
				fprintf (stderr, "ERROR: (file: %s | line: %d) \
			 fprintf() failed\n", __FILE__, __LINE__);
				exit (EXIT_FAILURE);
			}
			fclose (fp_game_stats);
			free (file_data);
			free (new_status_entry);
			/*	return from cb without destroying the window or restarting the game		*/
			return;
			}
		}
	/* Destroy the dialog after one of the above actions has taken place */
	gtk_widget_destroy (GTK_WIDGET (dialog));
	/*	restart game	*/
	retry_button_callback (NULL, NULL, user_data);
}




//Frage: will inline make progarm more perfomant??
/*function creates widgets for one line and adds them to grid*/
void code_breaker_add_new_row (GtkWidget *grid, gint row, struct widget_of_one_row *s_current_row, gint key, struct my_widgets *data)
{
	/*  create new widgets and set paramters for them*/
//new widget: label
	char buf[10];
	if( (sprintf (buf, "Round %d", row)) < 0)
	{
			fprintf (stderr, "ERROR: (file: %s | line: %d) \
		 sprintf() failed\n", __FILE__, __LINE__);
			exit (EXIT_FAILURE);
	}
	s_current_row->label = gtk_label_new (buf);
	gtk_widget_set_size_request (s_current_row->label, 100, 30);

//new widget: button_code1
	s_current_row->button_code1 = gtk_button_new_with_label ("0");
	gtk_widget_set_name (s_current_row->button_code1, "button_state0");
	gtk_widget_set_size_request (s_current_row->button_code1, 30, 30);
	g_signal_connect (s_current_row->button_code1, "clicked", G_CALLBACK (cb_code1_clicked_change_apperance), data);

	//new widget: button_code2
	s_current_row->button_code2 = gtk_button_new_with_label ("0");
	gtk_widget_set_name (s_current_row->button_code2, "button_state0");
	gtk_widget_set_size_request (s_current_row->button_code2, 30, 30);
	g_signal_connect (s_current_row->button_code2, "clicked", G_CALLBACK (cb_code2_clicked_change_apperance), data);

	//new widget: button_code3
	s_current_row->button_code3 = gtk_button_new_with_label ("0");
	gtk_widget_set_name (s_current_row->button_code3, "button_state0");
	gtk_widget_set_size_request (s_current_row->button_code3, 30, 30);
	g_signal_connect (s_current_row->button_code3, "clicked", G_CALLBACK (cb_code3_clicked_change_apperance), data);

	//new widget: button_code4
	s_current_row->button_code4 = gtk_button_new_with_label ("0");
	gtk_widget_set_name (s_current_row->button_code4, "button_state0");
	gtk_widget_set_size_request (s_current_row->button_code4, 30, 30);
	g_signal_connect (s_current_row->button_code4, "clicked", G_CALLBACK (cb_code4_clicked_change_apperance), data);

	//new widget: button_pos_hits
	s_current_row->button_pos_hits = gtk_button_new_with_label ("0");
	gtk_widget_set_name (s_current_row->button_pos_hits, "?");
	gtk_widget_set_size_request (s_current_row->button_pos_hits, 30, 30);

	//new widget:button_col_hits
	s_current_row->button_col_hits = gtk_button_new_with_label ("0");
	gtk_widget_set_name (s_current_row->button_col_hits, "?");
	gtk_widget_set_size_request (s_current_row->button_col_hits, 30, 30);

	/*	dependatn on key argument widgets are greyed out or not 	*/
	if (key == GREY_OUT) {
		//FALSE->button not accessible, TRUE->accessible
		gtk_widget_set_sensitive (s_current_row->label, FALSE);
		gtk_widget_set_sensitive (s_current_row->button_code1, FALSE);
		gtk_widget_set_sensitive (s_current_row->button_code2, FALSE);
		gtk_widget_set_sensitive (s_current_row->button_code3, FALSE);
		gtk_widget_set_sensitive (s_current_row->button_code4, FALSE);
	}
	/*feedback buttons are greyed out everytime*/
	gtk_widget_set_sensitive (s_current_row->button_pos_hits, FALSE);
	gtk_widget_set_sensitive (s_current_row->button_col_hits, FALSE);
	/*	add new widgets to grid	*/
	gtk_grid_attach (GTK_GRID (grid), s_current_row->label, 0, row, 1, 1);
	gtk_grid_attach (GTK_GRID (grid), s_current_row->button_code1, 1, row, 1, 1);
	gtk_grid_attach (GTK_GRID (grid), s_current_row->button_code2, 2, row, 1, 1);
	gtk_grid_attach (GTK_GRID (grid), s_current_row->button_code3, 3, row, 1, 1);
	gtk_grid_attach (GTK_GRID (grid), s_current_row->button_code4, 4, row, 1, 1);
	gtk_grid_attach (GTK_GRID (grid), s_current_row->button_pos_hits, 5, row, 1, 1);
	gtk_grid_attach (GTK_GRID (grid), s_current_row->button_col_hits, 6, row, 1, 1);
}

//##################################################################################################################
//------------------------------------------------------------------------------------------------------------------
//##################################################################################################################




//evaluate how many color and/or position are correct
//set button_col_hits and button_pos_hits accordingly
//grey out all widgets of current line, and make the ones of the next line accessible
void evaluate_curent_line_make_next_line_accessible_to_user (struct widget_of_one_row *s_current_row,
							     struct widget_of_one_row *s_next_row, gchar *status, struct my_widgets *data)
{
	gchar col_hits = 0, pos_hits = 0;
	gchar numeric_value_as_string[16]; //makes assumption that int is not bigger than 64 bit on our system
	gchar *value_button1, *value_button2, *value_button3, *value_button4;
	GtkWidget *dialog;
	GtkWidget *grid;
	GtkWidget *label;
	GtkWidget *content_area;
	/*	get current button values	*/
	value_button1 = (gchar*) gtk_button_get_label (GTK_BUTTON (s_current_row->button_code1));
	value_button2 = (gchar*) gtk_button_get_label (GTK_BUTTON (s_current_row->button_code2));
	value_button3 = (gchar*) gtk_button_get_label (GTK_BUTTON (s_current_row->button_code3));
	value_button4 = (gchar*) gtk_button_get_label (GTK_BUTTON (s_current_row->button_code4));

	/*	CHECK to make sure user has not entered any values twice ->inform and rerun line if he has done so	*/
	if (*value_button1 == *value_button2 || *value_button1 == *value_button3 || *value_button1 == *value_button4 || \
	    *value_button2 == *value_button3 || *value_button2 == *value_button4 || *value_button3 == *value_button4) {
		//inform user

		//set current line back to default
		gtk_button_set_label (GTK_BUTTON (s_current_row->button_code1), "0");
		gtk_widget_set_name (s_current_row->button_code1, "button_state0");
		gtk_button_set_label (GTK_BUTTON (s_current_row->button_code2), "0");
		gtk_widget_set_name (s_current_row->button_code2, "button_state0");
		gtk_button_set_label (GTK_BUTTON (s_current_row->button_code3), "0");
		gtk_widget_set_name (s_current_row->button_code3, "button_state0");
		gtk_button_set_label (GTK_BUTTON (s_current_row->button_code4), "0");
		gtk_widget_set_name (s_current_row->button_code4, "button_state0");

		//status failed
		*status = -1;
	} else {
		//status OK
		*status = 0;
		/*	identifier color hits	*/
		if (*value_button1 == data->code[0])
			pos_hits++;
		if (*value_button2 == data->code[1])
			pos_hits++;
		if (*value_button3 == data->code[2])
			pos_hits++;
		if (*value_button4 == data->code[3])
			pos_hits++;

		if (*value_button1 == data->code[0] || *value_button1 == data->code[1] || *value_button1 == data->code[2]\
		    || *value_button1 == data->code[3])
			col_hits++;
		if (*value_button2 == data->code[0] || *value_button2 == data->code[1] || *value_button2 == data->code[2]\
		    || *value_button2 == data->code[3])
			col_hits++;
		if (*value_button3 == data->code[0] || *value_button3 == data->code[1] || *value_button3 == data->code[2]\
		    || *value_button3 == data->code[3])
			col_hits++;
		if (*value_button4 == data->code[0] || *value_button4 == data->code[1] || *value_button4 == data->code[2]\
		    || *value_button4 == data->code[3])
			col_hits++;

		/*	set button_col_hits and button_pos_hits accordingly	*/
		//cast numeric value to string
		if( (sprintf (numeric_value_as_string, "%d", col_hits)) < 0)
		{
			fprintf (stderr, "ERROR: (file: %s | line: %d) \
		 sprintf() failed\n", __FILE__, __LINE__);
			exit (EXIT_FAILURE);
		}
		gtk_button_set_label (GTK_BUTTON (s_current_row->button_col_hits), (const gchar*) numeric_value_as_string);
		if( (sprintf (numeric_value_as_string, "%d", pos_hits)) < 0)
		{
			fprintf (stderr, "ERROR: (file: %s | line: %d) \
		 sprintf() failed\n", __FILE__, __LINE__);
			exit (EXIT_FAILURE);
		}
		gtk_button_set_label (GTK_BUTTON (s_current_row->button_pos_hits), (const gchar*) numeric_value_as_string);

		/*	check to see if user has broken the code, if so ->inform player that he has won, and restart game again in background*/
		if (pos_hits == 4) {

			/* Create a new dialog, and set the parameters as follows:
			 * Dialog Flags - make the constructed dialog modal
			 * (modal windows prevent interaction with other windows in the application)
			 * Buttons Type - cancel button makro definded in glib
			 * as well as user defined makros (just for showcase)
			 */
			dialog = gtk_dialog_new_with_buttons ("Congratulations you won the game",
							      GTK_WINDOW (data->window),
										GTK_DIALOG_MODAL,
										_("save stats"),
										DIALOG_RESPONSE_SAVE_STATS,
							      _("show stats"),
							      DIALOG_RESPONSE_SHOW_STATS,
							      _("close and play again"),
							      GTK_RESPONSE_CANCEL,
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
			label = gtk_label_new ("to save stats enter your name and press <save stats>\
			to quit without saving them, press <quit>");
			gtk_grid_attach (GTK_GRID (grid), label, 0, 0, 1, 1);
			data->entry = gtk_entry_new();
			gtk_grid_attach (GTK_GRID (grid), data->entry, 0, 1, 1, 1);
			/* add some space below the content area and above the buttons */
			gtk_widget_set_margin_bottom (grid, 20);
			/* display the message dialog */
			gtk_widget_show_all (dialog);
			/* connect the response signal(emitted when anny button is pressed)->the specific signals emmitted from the buttons inside dialog
			can then be idnetified via their respective macro int value (response id)*/
			g_signal_connect (GTK_DIALOG (dialog), "response", G_CALLBACK (on_response), data);

		}

		if (s_next_row->button_code1 != NULL) {
			/* grey out all widgets of current line, and make the ones of the next line accessible*/
			gtk_widget_set_sensitive (s_current_row->button_code1, FALSE);
			gtk_widget_set_sensitive (s_current_row->button_code2, FALSE);
			gtk_widget_set_sensitive (s_current_row->button_code3, FALSE);
			gtk_widget_set_sensitive (s_current_row->button_code4, FALSE);

			gtk_widget_set_sensitive (s_next_row->button_code1, TRUE);
			gtk_widget_set_sensitive (s_next_row->button_code2, TRUE);
			gtk_widget_set_sensitive (s_next_row->button_code3, TRUE);
			gtk_widget_set_sensitive (s_next_row->button_code4, TRUE);
		} else {
			exit (EXIT_SUCCESS);
		}

	}
}


//##################################################################################################################
//------------------------------------------------------------------------------------------------------------------
//##################################################################################################################

//##################################################################################################################
//------------------------------------------------------------------------------------------------------------------
//##################################################################################################################


// callback that is executed when one of the "code" buttons in the body is pressed
void cb_code1_clicked_change_apperance (GtkWidget *button, gpointer user_data)
{
	struct my_widgets *data = (struct my_widgets*) user_data;
	//(void*) user_data; //->no need to typecast to void -gpointer- is synonym for void*
	static gint button_state = 1; //static variable declaration ->will hold value form one func call to next

	/*****callback is invoked from submit_button or rerun_button  clicked ->reset counter for new line******/
	if (user_data == NULL) {
		button_state = 1;
		return;
	}
	/***********************************************/
	if (0 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "0");
		gtk_widget_set_name (button, "button_state0");
	}
	if (1 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "1");
		//by setting the name, we change the button theme via our css file
		gtk_widget_set_name (button, "button_state1");
	}
	if (2 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "2");
		gtk_widget_set_name (button, "button_state2");
	}
	if (3 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "3");
		gtk_widget_set_name (button, "button_state3");
	}
	if (4 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "4");
		gtk_widget_set_name (button, "button_state4");
	}
	if (5 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "5");
		gtk_widget_set_name (button, "button_state5");
	}
	if (6 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "6");
		gtk_widget_set_name (button, "button_state6");
	}
	if (7 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "7");
		gtk_widget_set_name (button, "button_state7");
	}


	button_state++;
	if ( (4 + (data->alph_difficulty) * 2) == button_state)
		button_state = 0;
}

// callback that is executed when one of the "code" buttons in the body is pressed
void cb_code2_clicked_change_apperance (GtkWidget *button, gpointer user_data)
{
	struct my_widgets *data = (struct my_widgets*) user_data;
	//(void*) user_data; //->no need to typecast to void -gpointer- is synonym for void*
	static gint button_state = 1; //static variable declaration ->will hold value form one func call to next

	/*****callback is invoked from submit_button or rerun_button  clicked ->reset counter for new line******/
	if (user_data == NULL) {
		button_state = 1;
		return;
	}
	/***********************************************/
	if (0 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "0");
		gtk_widget_set_name (button, "button_state0");
	}
	if (1 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "1");
		//by setting the name, we change the button theme via our css file
		gtk_widget_set_name (button, "button_state1");
	}
	if (2 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "2");
		gtk_widget_set_name (button, "button_state2");
	}
	if (3 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "3");
		gtk_widget_set_name (button, "button_state3");
	}
	if (4 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "4");
		gtk_widget_set_name (button, "button_state4");
	}
	if (5 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "5");
		gtk_widget_set_name (button, "button_state5");
	}
	if (6 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "6");
		gtk_widget_set_name (button, "button_state6");
	}
	if (7 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "7");
		gtk_widget_set_name (button, "button_state7");
	}


	button_state++;
	if ( (4 + (data->alph_difficulty) * 2) == button_state)
		button_state = 0;
}

// callback that is executed when one of the "code" buttons in the body is pressed
void cb_code3_clicked_change_apperance (GtkWidget *button, gpointer user_data)
{
	struct my_widgets *data = (struct my_widgets*) user_data;
	//(void*) user_data; //->no need to typecast to void -gpointer- is synonym for void*
	static gint button_state = 1; //static variable declaration ->will hold value form one func call to next

	/*****callback is invoked from submit_button or rerun_button  clicked ->reset counter for new line******/
	if (user_data == NULL) {
		button_state = 1;
		return;
	}
	/***********************************************/
	if (0 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "0");
		gtk_widget_set_name (button, "button_state0");
	}
	if (1 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "1");
		//by setting the name, we change the button theme via our css file
		gtk_widget_set_name (button, "button_state1");
	}
	if (2 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "2");
		gtk_widget_set_name (button, "button_state2");
	}
	if (3 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "3");
		gtk_widget_set_name (button, "button_state3");
	}
	if (4 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "4");
		gtk_widget_set_name (button, "button_state4");
	}
	if (5 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "5");
		gtk_widget_set_name (button, "button_state5");
	}
	if (6 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "6");
		gtk_widget_set_name (button, "button_state6");
	}
	if (7 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "7");
		gtk_widget_set_name (button, "button_state7");
	}


	button_state++;
	if ( (4 + (data->alph_difficulty) * 2) == button_state)
		button_state = 0;
}

// callback that is executed when one of the "code" buttons in the body is pressed
void cb_code4_clicked_change_apperance (GtkWidget *button, gpointer user_data)
{
	struct my_widgets *data = (struct my_widgets*) user_data;
	//(void*) user_data; //->no need to typecast to void -gpointer- is synonym for void*
	static gint button_state = 1; //static variable declaration ->will hold value form one func call to next

	/*****callback is invoked from submit_button or rerun_button clicked ->reset counter for new line******/
	if (user_data == NULL) {
		button_state = 1;
		return;
	}
	/***********************************************/
	if (0 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "0");
		gtk_widget_set_name (button, "button_state0");
	}
	if (1 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "1");
		//by setting the name, we change the button theme via our css file
		gtk_widget_set_name (button, "button_state1");
	}
	if (2 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "2");
		gtk_widget_set_name (button, "button_state2");
	}
	if (3 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "3");
		gtk_widget_set_name (button, "button_state3");
	}
	if (4 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "4");
		gtk_widget_set_name (button, "button_state4");
	}
	if (5 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "5");
		gtk_widget_set_name (button, "button_state5");
	}
	if (6 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "6");
		gtk_widget_set_name (button, "button_state6");
	}
	if (7 == button_state) {
		gtk_button_set_label (GTK_BUTTON (button), "7");
		gtk_widget_set_name (button, "button_state7");
	}


	button_state++;
	if ( (4 + (data->alph_difficulty) * 2) == button_state)
		button_state = 0;
}

/*
	TAKEN FROM WEB
	https://www.geeksforgeeks.org/shuffle-a-given-array-using-fisher-yates-shuffle-algorithm/
*/
// A utility function to swap to array elements
static void swap (gchar *a, gchar *b)
{
	gchar temp = *a;
	*a = *b;
	*b = temp;
}
// A function to generate a random
// permutation of arr[]
void randomize (gchar arr[], int n)
{
	// Use a different seed value so that
	// we don't get same result each time
	// we run this program
	srand (time (NULL));

	// Start from the last element and swap
	// one by one. We don't need to run for
	// the first element that's why i > 0
	for (int i = n - 1; i > 0; i--) {
		// Pick a random index from 0 to i
		int j = rand() % (i + 1);

		// Swap arr[i] with the element
		// at random index
		swap (&arr[i], &arr[j]);
	}
}







/****************************************************************** CALLBACKS */
gint TimerCallback (gpointer data)
{
	struct my_widgets *user_data = (struct my_widgets*) data;
	static guint statusbar_entry_id = 0;
	guint status_bar_contix_id = gtk_statusbar_get_context_id (GTK_STATUSBAR (user_data->statusbar), "MY STATUS BAR");
	gchar msg[300]; //it is estimated that the string probaly wont get lager than 300 characters
	/* --- Another second has gone by --- */


	user_data->gametime++;

	if( (g_snprintf (msg, 300, "Rounds left: %d\tTime elapsed: %d(seconds)", \
		    ( (8 + 2 * (user_data->current_tries_difficulty)) - user_data->current_row), user_data->gametime)) < 0)
				{
					fprintf (stderr, "ERROR: (file: %s | line: %d) g_sprintf() failed\n", __FILE__, __LINE__);
					exit (EXIT_FAILURE);
				}
	statusbar_entry_id = gtk_statusbar_push (GTK_STATUSBAR (user_data->statusbar), user_data->id, msg);
	gtk_statusbar_remove (GTK_STATUSBAR (user_data->statusbar),status_bar_contix_id, statusbar_entry_id);
	return 1;
}




void
submit_button_callback (GSimpleAction *action, GVariant *parameter, gpointer data)
{
#if FS_DEBUG
	g_print ("submit_button_callback was envoced ...\n");
#endif
	(void)action;
	(void)parameter;
	struct my_widgets *widget_ptr = (struct my_widgets*) data;
	gchar status = 0;
	GtkWidget *dialog;
	//save current game time, in case user has won, as timer will run on
	widget_ptr->seconds_it_took = widget_ptr->gametime;
	switch (widget_ptr->current_row) {
	case 0:
		evaluate_curent_line_make_next_line_accessible_to_user (& (widget_ptr->row1), & (widget_ptr->row2), &status, widget_ptr);
		break;
	case 1:
		evaluate_curent_line_make_next_line_accessible_to_user (& (widget_ptr->row2), & (widget_ptr->row3), &status, widget_ptr);
		break;
	case 2:
		evaluate_curent_line_make_next_line_accessible_to_user (& (widget_ptr->row3), & (widget_ptr->row4), &status, widget_ptr);
		break;
	case 3:
		evaluate_curent_line_make_next_line_accessible_to_user (& (widget_ptr->row4), & (widget_ptr->row5), &status, widget_ptr);
		break;
	case 4:
		evaluate_curent_line_make_next_line_accessible_to_user (& (widget_ptr->row5), & (widget_ptr->row6), &status, widget_ptr);
		break;
	case 5:
		evaluate_curent_line_make_next_line_accessible_to_user (& (widget_ptr->row6), & (widget_ptr->row7), &status, widget_ptr);
		break;
	case 6:
		evaluate_curent_line_make_next_line_accessible_to_user (& (widget_ptr->row7), & (widget_ptr->row8), &status, widget_ptr);
		break;
	case 7:
		evaluate_curent_line_make_next_line_accessible_to_user (& (widget_ptr->row8), & (widget_ptr->row9), &status, widget_ptr);
		if(widget_ptr->current_tries_difficulty == 0) //hard tries difficulty ->last line reached
		{
			//popup ->you lost, game will restart
			dialog = gtk_message_dialog_new (GTK_WINDOW (widget_ptr->window),
							 GTK_DIALOG_MODAL,
							 GTK_MESSAGE_INFO,
							 GTK_BUTTONS_CLOSE,
							 "YOU LOST THE GAME\
					 \rgood luck next time\n to restart game, close this window\n");
			g_signal_connect (dialog, "response", G_CALLBACK (on_response_2), widget_ptr);
			gtk_widget_show (dialog);
		}
		break;
	case 8:
		evaluate_curent_line_make_next_line_accessible_to_user (& (widget_ptr->row9), & (widget_ptr->row10), &status, widget_ptr);
		break;
	case 9:
		evaluate_curent_line_make_next_line_accessible_to_user (& (widget_ptr->row10), & (widget_ptr->row11), &status, widget_ptr);
		break;
	case 10:
		evaluate_curent_line_make_next_line_accessible_to_user (& (widget_ptr->row11), & (widget_ptr->row12), &status, widget_ptr);
		break;
	case 11:
		evaluate_curent_line_make_next_line_accessible_to_user (& (widget_ptr->row12), NULL, &status, widget_ptr);
		break;
	default:
		exit (EXIT_FAILURE);
		break;
	}
	if (status == 0) //only increment if status is ok
		widget_ptr->current_row++;
	//popup that informs user about game logic
	if (status != 0) {
		dialog = gtk_message_dialog_new (GTK_WINDOW (widget_ptr->window),
						 GTK_DIALOG_DESTROY_WITH_PARENT,
						 GTK_MESSAGE_INFO,
						 GTK_BUTTONS_CLOSE,
						 "NOTE:\
				 \ryou entered a code with one or more values of same type, but the code does not contain any value twice.\
				 \rThe line was reset for you, try again, now with no value appearing more than once!");
		g_signal_connect (dialog, "response", G_CALLBACK (gtk_widget_destroy), NULL);
		gtk_widget_show (dialog);
	}
}




//simple callback that closes window and restarts game
void
on_response_2 (GtkDialog *dialog, gint response_id, gpointer user_data)
{
	(void)response_id;
	/* Destroy the dialog  */
	gtk_widget_destroy (GTK_WIDGET (dialog));
	/*	restart game	*/
	retry_button_callback (NULL, NULL, user_data);
}











void
retry_button_callback (GSimpleAction *action, GVariant *parameter, gpointer data)
{
#if FS_DEBUG
	g_print ("retry_button_callback was envoced ...\n");
#endif
(void)action;
(void)parameter;
	struct my_widgets *user_data = (struct my_widgets*) data;
	//reset row counter
	user_data->current_row = 0;
	//reset game Timer
	user_data->gametime = 0;

//delete all previous lines
	for (gint j = 0; j <= 8 + (user_data->prev_tries_difficulty) * 2 + 1; j++) {
		gtk_grid_remove_row (GTK_GRID (user_data->grid), 0);
	}

//build new ones
//add new lines dependant on current_tries_difficulty
	switch (user_data->current_tries_difficulty) {
	case 0:
		code_breaker_add_new_row (user_data->grid, 1, & (user_data->row1), 0, user_data);
		code_breaker_add_new_row (user_data->grid, 2, & (user_data->row2), GREY_OUT, user_data);
		code_breaker_add_new_row (user_data->grid, 3, & (user_data->row3), GREY_OUT, user_data);
		code_breaker_add_new_row (user_data->grid, 4, & (user_data->row4), GREY_OUT, user_data);
		code_breaker_add_new_row (user_data->grid, 5, & (user_data->row5), GREY_OUT, user_data);
		code_breaker_add_new_row (user_data->grid, 6, & (user_data->row6), GREY_OUT, user_data);
		code_breaker_add_new_row (user_data->grid, 7, & (user_data->row7), GREY_OUT, user_data);
		code_breaker_add_new_row (user_data->grid, 8, & (user_data->row8), GREY_OUT, user_data);
		break;
	case 1:
		code_breaker_add_new_row (user_data->grid, 1, & (user_data->row1), 0, user_data);
		code_breaker_add_new_row (user_data->grid, 2, & (user_data->row2), GREY_OUT, user_data);
		code_breaker_add_new_row (user_data->grid, 3, & (user_data->row3), GREY_OUT, user_data);
		code_breaker_add_new_row (user_data->grid, 4, & (user_data->row4), GREY_OUT, user_data);
		code_breaker_add_new_row (user_data->grid, 5, & (user_data->row5), GREY_OUT, user_data);
		code_breaker_add_new_row (user_data->grid, 6, & (user_data->row6), GREY_OUT, user_data);
		code_breaker_add_new_row (user_data->grid, 7, & (user_data->row7), GREY_OUT, user_data);
		code_breaker_add_new_row (user_data->grid, 8, & (user_data->row8), GREY_OUT, user_data);
		code_breaker_add_new_row (user_data->grid, 9, & (user_data->row9), GREY_OUT, user_data);
		code_breaker_add_new_row (user_data->grid, 10, & (user_data->row10), GREY_OUT, user_data);
		break;
	case 2:
		code_breaker_add_new_row (user_data->grid, 1, & (user_data->row1), 0, user_data);
		code_breaker_add_new_row (user_data->grid, 2, & (user_data->row2), GREY_OUT, user_data);
		code_breaker_add_new_row (user_data->grid, 3, & (user_data->row3), GREY_OUT, user_data);
		code_breaker_add_new_row (user_data->grid, 4, & (user_data->row4), GREY_OUT, user_data);
		code_breaker_add_new_row (user_data->grid, 5, & (user_data->row5), GREY_OUT, user_data);
		code_breaker_add_new_row (user_data->grid, 6, & (user_data->row6), GREY_OUT, user_data);
		code_breaker_add_new_row (user_data->grid, 7, & (user_data->row7), GREY_OUT, user_data);
		code_breaker_add_new_row (user_data->grid, 8, & (user_data->row8), GREY_OUT, user_data);
		code_breaker_add_new_row (user_data->grid, 9, & (user_data->row9), GREY_OUT, user_data);
		code_breaker_add_new_row (user_data->grid, 10, & (user_data->row10), GREY_OUT, user_data);
		code_breaker_add_new_row (user_data->grid, 11, & (user_data->row11), GREY_OUT, user_data);
		code_breaker_add_new_row (user_data->grid, 12, & (user_data->row12), GREY_OUT, user_data);
		break;
	default:
		fprintf (stderr, "ERROR: (file: %s | line: %d) \
		switch() failed\n", __FILE__, __LINE__);
		exit (EXIT_FAILURE);
		break;
	}
	/* Statusbar */
	user_data->statusbar = gtk_statusbar_new();
	gtk_grid_attach (GTK_GRID (user_data->grid), user_data->statusbar, 1, (9 +  2 * (user_data->current_tries_difficulty)), 7, 1);
	user_data->id = gtk_statusbar_get_context_id (GTK_STATUSBAR (user_data->statusbar), "demo");

//display newly built grid
	gtk_widget_show_all (user_data->window);

//generate new code
	user_data->code[0] = '0';
	user_data->code[1] = '1';
	user_data->code[2] = '2';
	user_data->code[3] = '3';
	user_data->code[4] = '4';
	user_data->code[5] = '5';
	user_data->code[6] = '6';
	user_data->code[7] = '7';
	randomize (user_data->code, 4 + ( (user_data->alph_difficulty) * 2));
#if FS_DEBUG
	for (int i = 0; i < 4 ; i++)
		printf ("code: %c\n", user_data->code[i]);
	printf ("--------------------------");
#endif


}







void
alph_easy_callback (GSimpleAction *action, GVariant *parameter, gpointer data)
{
#if FS_DEBUG
	g_print ("alph_easy_callback was envoced ...\n");
#endif
(void)action;
(void)parameter;
	struct my_widgets *user_data = (struct my_widgets*) data;
	user_data->prev_tries_difficulty = user_data->current_tries_difficulty;
	user_data->alph_difficulty = 0;
	/*	restart game with new settings	*/
	retry_button_callback (NULL, NULL, data);
}
void
alph_med_callback (GSimpleAction *action, GVariant *parameter, gpointer data)
{
#if FS_DEBUG
	g_print ("alph_med_callback was envoced ...\n");
#endif
(void)action;
(void)parameter;
	struct my_widgets *user_data = (struct my_widgets*) data;
	user_data->prev_tries_difficulty = user_data->current_tries_difficulty;
	user_data->alph_difficulty = 1;
	/*	restart game with new settings	*/
	retry_button_callback (NULL, NULL, data);
}
void
alph_hard_callback (GSimpleAction *action, GVariant *parameter, gpointer data)
{
#if FS_DEBUG
	g_print ("alph_hard_callback was envoced ...\n");
#endif
(void)action;
(void)parameter;
	struct my_widgets *user_data = (struct my_widgets*) data;
	user_data->prev_tries_difficulty = user_data->current_tries_difficulty;
	user_data->alph_difficulty = 2;
	/*	restart game with new settings	*/
	retry_button_callback (NULL, NULL, data);
}

void
tries_easy_callback (GSimpleAction *action, GVariant *parameter, gpointer data)
{
#if FS_DEBUG
	g_print ("tries_easy_callback was envoced ...\n");
#endif
(void)action;
(void)parameter;
	struct my_widgets *user_data = (struct my_widgets*) data;
	user_data->prev_tries_difficulty = user_data->current_tries_difficulty;
	user_data->current_tries_difficulty = 2;
	/*	restart game with new settings	*/
	retry_button_callback (NULL, NULL, data);
}
void
tries_med_callback (GSimpleAction *action, GVariant *parameter, gpointer data)
{
#if FS_DEBUG
	g_print ("tries_med_callback was envoced ...\n");
#endif
(void)action;
(void)parameter;
	struct my_widgets *user_data = (struct my_widgets*) data;
	user_data->prev_tries_difficulty = user_data->current_tries_difficulty;
	user_data->current_tries_difficulty = 1;
	/*	restart game with new settings	*/
	retry_button_callback (NULL, NULL, data);
}
void
tries_hard_callback (GSimpleAction *action, GVariant *parameter, gpointer data)
{
#if FS_DEBUG
	g_print ("tries_hard_callback was envoced ...\n");
#endif
(void)action;
(void)parameter;
	struct my_widgets *user_data = (struct my_widgets*) data;
	user_data->prev_tries_difficulty = user_data->current_tries_difficulty;
	user_data->current_tries_difficulty = 0;
	/*	restart game with new settings	*/
	retry_button_callback (NULL, NULL, data);
}


void
about_callback (GSimpleAction *action, GVariant *parameter, gpointer data)
{
#if FS_DEBUG
	g_print ("about_callback was envoced ...\n");
#endif
(void)action;
(void)parameter;
	struct my_widgets *a = (struct my_widgets *) data; //typecast back from void*
	GtkWidget *dialog;
	FILE *fp_readme;
	long file_size_in_byte;
	char *file_data;

	/*****open file in binary mode to get meaninfull return from ftell() call  */
	fp_readme = fopen (TEXTFILE_README, "rb");
	if (fp_readme == NULL) {
		fprintf (stderr, "ERROR: (file: %s | line: %d) \
	 fopen() of game_statistics.txt failed\n", __FILE__, __LINE__);
		exit (EXIT_FAILURE);
	}
	/*****get file size in byte   */
	//move to end of file
	if( (fseek (fp_readme, 0, SEEK_END)) != 0)
	{
		fclose (fp_readme);
		fprintf (stderr, "ERROR: (file: %s | line: %d) \
	 fseek() failed\n", __FILE__, __LINE__);
		exit (EXIT_FAILURE);
	}
	//get file positon indicator
	if( (file_size_in_byte = ftell (fp_readme)) == EOF)
	{
		fclose (fp_readme);
		fprintf (stderr, "ERROR: (file: %s | line: %d) \
	 ftell() failed\n", __FILE__, __LINE__);
		exit (EXIT_FAILURE);
	}
	//move file postion back to beginning
	rewind (fp_readme);
	/*****allocate memory for buffer to hold file content*/
	file_data = calloc (1, file_size_in_byte + 1);
	if (file_data == NULL) {
		fclose (fp_readme);
		fprintf (stderr, "ERROR: (file: %s | line: %d) \
	 allocation for file_data failed\n", __FILE__, __LINE__);
		exit (EXIT_FAILURE);
	}

	/*****read content from file and save to buffer*/
	if ( (fread (file_data, file_size_in_byte, 1, fp_readme)) != 1) {
		fclose (fp_readme);
		free (file_data);
		fprintf (stderr, "ERROR: (file: %s | line: %d) \
	 read() from game_statistics.txt failed\n", __FILE__, __LINE__);
		exit (EXIT_FAILURE);
	}

	/*****just be sure put an extra String terminator at the end*/
	file_data[file_size_in_byte + 2] = '\0';


	/*	Popup that displays short game description*/
	dialog = gtk_message_dialog_new (GTK_WINDOW (a->window),
					 GTK_DIALOG_DESTROY_WITH_PARENT,
					 GTK_MESSAGE_INFO,
					 GTK_BUTTONS_CLOSE,
					 "%s", (const gchar*) file_data);
	//connect popup with function -gtk_widget_destroy- from gtk library that will destroy it
	g_signal_connect (dialog, "response", G_CALLBACK (gtk_widget_destroy), NULL);
	gtk_widget_show (dialog);
	fclose (fp_readme);
	free (file_data);
}


void
stats_callback (GSimpleAction *action, GVariant *parameter, gpointer data)
{
#if FS_DEBUG
	g_print ("stats_callback was envoced ...\n");
#endif
(void)action;
(void)parameter;
	struct my_widgets *a = (struct my_widgets *) data; //typecast back from void*
	GtkWidget *dialog;
	FILE *fp_game_stats;
	long file_size_in_byte;
	char *file_data;

	/*****open file in binary mode to get meaninfull return from ftell() call  */
	fp_game_stats = fopen (TEXTFILE_GAME_STATS, "rb");
	if (fp_game_stats == NULL) {
		fprintf (stderr, "ERROR: (file: %s | line: %d) \
   fopen() of game_statistics.txt failed\n", __FILE__, __LINE__);
		exit (EXIT_FAILURE);
	}
	/*****get file size in byte   */
//move to end of file
	if( (fseek (fp_game_stats, 0, SEEK_END)) != 0)
	{
		fclose (fp_game_stats);
		fprintf (stderr, "ERROR: (file: %s | line: %d) \
	 fseek() failed\n", __FILE__, __LINE__);
		exit (EXIT_FAILURE);
	}
//get file positon indicator
	if( (file_size_in_byte = ftell (fp_game_stats)) == EOF)
	{
		fclose (fp_game_stats);
		fprintf (stderr, "ERROR: (file: %s | line: %d) \
	 ftell() failed\n", __FILE__, __LINE__);
		exit (EXIT_FAILURE);
	}
//move file postion back to beginning
	rewind (fp_game_stats);
	/*****allocate memory for buffer to hold file content*/
	file_data = calloc (1, file_size_in_byte + 1);
	if (file_data == NULL) {
		fclose (fp_game_stats);
		fprintf (stderr, "ERROR: (file: %s | line: %d) \
   allocation for file_data failed\n", __FILE__, __LINE__);
		exit (EXIT_FAILURE);
	}

	/*****read conten from file and save to buffer*/
	if ( (fread (file_data, file_size_in_byte, 1, fp_game_stats)) != 1) {
		fclose (fp_game_stats);
		free (file_data);
		fprintf (stderr, "ERROR: (file: %s | line: %d) \
   read() from game_statistics.txt failed\n", __FILE__, __LINE__);
		exit (EXIT_FAILURE);
	}

	/*****just be sure put an extra String terminator at the end*/
	file_data[file_size_in_byte + 2] = '\0';


	/*	Popup that displays short game description*/
	dialog = gtk_message_dialog_new (GTK_WINDOW (a->window),
					 GTK_DIALOG_DESTROY_WITH_PARENT,
					 GTK_MESSAGE_INFO,
					 GTK_BUTTONS_CLOSE,
					 "%s", (const gchar*) file_data);
	//connect popup with function -gtk_widget_destroy- from gtk library that will destroy it
	g_signal_connect (dialog, "response", G_CALLBACK (gtk_widget_destroy), NULL);
	gtk_widget_show (dialog);
	fclose (fp_game_stats);
	free (file_data);
}

/** EOF */

















/** EOF */
