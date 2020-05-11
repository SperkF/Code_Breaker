
#include "functions.h"

//Frage: will inline make progarm more perfomant??
/*function creates widgets for one line and adds them to grid*/
void code_breaker_add_new_row (GtkWidget *grid, gint row, struct widget_of_one_row *s_current_row, gint key)
{
	/*  create new widgets and set paramters for them*/
//new widget: label
	s_current_row->label = gtk_label_new ("Round 1");
	gtk_widget_set_size_request (s_current_row->label, 100, 30);

//new widget: button_code1
	s_current_row->button_code1 = gtk_button_new_with_label ("?");
	gtk_widget_set_name (s_current_row->button_code1, "button_state0");
	gtk_widget_set_size_request (s_current_row->button_code1, 30, 30);
	g_signal_connect (s_current_row->button_code1, "clicked", G_CALLBACK (cb_code1_clicked_change_apperance), NULL);

	//new widget: button_code2
	s_current_row->button_code2 = gtk_button_new_with_label ("?");
	gtk_widget_set_name (s_current_row->button_code2, "button_state0");
	gtk_widget_set_size_request (s_current_row->button_code2, 30, 30);
	g_signal_connect (s_current_row->button_code2, "clicked", G_CALLBACK (cb_code2_clicked_change_apperance), NULL);

	//new widget: button_code3
	s_current_row->button_code3 = gtk_button_new_with_label ("?");
	gtk_widget_set_name (s_current_row->button_code3, "button_state0");
	gtk_widget_set_size_request (s_current_row->button_code3, 30, 30);
	g_signal_connect (s_current_row->button_code3, "clicked", G_CALLBACK (cb_code3_clicked_change_apperance), NULL);

	//new widget: button_code4
	s_current_row->button_code4 = gtk_button_new_with_label ("?");
	gtk_widget_set_name (s_current_row->button_code4, "button_state0");
	gtk_widget_set_size_request (s_current_row->button_code4, 30, 30);
	g_signal_connect (s_current_row->button_code4, "clicked", G_CALLBACK (cb_code4_clicked_change_apperance), NULL);

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
							     struct widget_of_one_row *s_next_row, gchar *status)
{
	gchar col_hits = 0, pos_hits = 0;

	gchar numeric_value_as_string[16]; //makes assumption that int is not bigger than 64 bit on our system

	gchar *value_button1, *value_button2, *value_button3, *value_button4;
	gchar code[4] = {'1','2','3','0'};
	/*	get current button values	*/
	value_button1 = (gchar*) gtk_button_get_label (GTK_BUTTON (s_current_row->button_code1));
	value_button2 = (gchar*) gtk_button_get_label (GTK_BUTTON (s_current_row->button_code2));
	value_button3 = (gchar*) gtk_button_get_label (GTK_BUTTON (s_current_row->button_code3));
	value_button4 = (gchar*) gtk_button_get_label (GTK_BUTTON (s_current_row->button_code4));

	/*	CHECK to make sure user has not entered any values twice ->inform and rerung line if he has done so	*/
	if (*value_button1 == *value_button2 || *value_button1 == *value_button3 || *value_button1 == *value_button4 ||\
	    *value_button2 == *value_button3 || *value_button2 == *value_button4 || *value_button3 == *value_button4) {
		//inform user

		//set current line back to default
		gtk_button_set_label (GTK_BUTTON (s_current_row->button_code1), "?");
		gtk_widget_set_name (s_current_row->button_code1, "button_state0");
		gtk_button_set_label (GTK_BUTTON (s_current_row->button_code2), "?");
		gtk_widget_set_name (s_current_row->button_code2, "button_state0");
		gtk_button_set_label (GTK_BUTTON (s_current_row->button_code3), "?");
		gtk_widget_set_name (s_current_row->button_code3, "button_state0");
		gtk_button_set_label (GTK_BUTTON (s_current_row->button_code4), "?");
		gtk_widget_set_name (s_current_row->button_code4, "button_state0");

		//status failed
		*status = -1;
	} else {
		//status OK
		*status = 0;
		/*	identifier color hits	*/
		if (*value_button1 == '1'|| *value_button1 == '2' || *value_button1 == '3' || *value_button1 == '4')
			col_hits++;

		/*	set button_col_hits and button_pos_hits accordingly	*/
		//cast numeric value to string
		sprintf (numeric_value_as_string, "%d", col_hits);
		gtk_button_set_label (GTK_BUTTON (s_current_row->button_col_hits), (const gchar*) numeric_value_as_string);
		sprintf (numeric_value_as_string, "%d", col_hits);
		gtk_button_set_label (GTK_BUTTON (s_current_row->button_pos_hits), (const gchar*) numeric_value_as_string);

		/*	check to see if user has broken the code, if so ->terminate game and inform player that he has won*/
		/*
		if(...)
		{
			//isert functionality here ->POP-UP??
		}
		*/
		/*	check to see if last line has been reached, if so ->terminate game and inform player that he has lost*/
		if (s_next_row == NULL) {
			exit (EXIT_SUCCESS);
			//isert functionality here ->POP-UP??
		} else {
			/* grey out all widgets of current line, and make the ones of the next line accessible*/
			gtk_widget_set_sensitive (s_current_row->button_code1, FALSE);
			gtk_widget_set_sensitive (s_current_row->button_code2, FALSE);
			gtk_widget_set_sensitive (s_current_row->button_code3, FALSE);
			gtk_widget_set_sensitive (s_current_row->button_code4, FALSE);

			gtk_widget_set_sensitive (s_next_row->button_code1, TRUE);
			gtk_widget_set_sensitive (s_next_row->button_code2, TRUE);
			gtk_widget_set_sensitive (s_next_row->button_code3, TRUE);
			gtk_widget_set_sensitive (s_next_row->button_code4, TRUE);
		}
	}
}


//##################################################################################################################
//------------------------------------------------------------------------------------------------------------------
//##################################################################################################################



// callback that is executed when the "test" button in the menu is pressed
void test_button_clicked (GtkWidget *button, gpointer user_data)
{
	static gint call_counter = 0; //to know in wich row we are in ->call_counter = row
	gchar status = 0;
	struct my_widgets *widget_ptr = (struct my_widgets*) user_data;
	switch (call_counter) {
	case 0:
		evaluate_curent_line_make_next_line_accessible_to_user (& (widget_ptr->row1), & (widget_ptr->row2), &status);
		break;
	case 1:
		evaluate_curent_line_make_next_line_accessible_to_user (& (widget_ptr->row2), & (widget_ptr->row3), &status);
		break;
	case 2:
		evaluate_curent_line_make_next_line_accessible_to_user (& (widget_ptr->row3), & (widget_ptr->row4), &status);
		break;
	case 3:
		evaluate_curent_line_make_next_line_accessible_to_user (& (widget_ptr->row4), & (widget_ptr->row5), &status);
		break;
	case 4:
		evaluate_curent_line_make_next_line_accessible_to_user (& (widget_ptr->row5), & (widget_ptr->row6), &status);
		break;
	case 5:
		evaluate_curent_line_make_next_line_accessible_to_user (& (widget_ptr->row6), & (widget_ptr->row7), &status);
		break;
	case 6:
		evaluate_curent_line_make_next_line_accessible_to_user (& (widget_ptr->row7), & (widget_ptr->row8), &status);
		break;
	case 7:
		evaluate_curent_line_make_next_line_accessible_to_user (& (widget_ptr->row8), & (widget_ptr->row9), &status);
		break;
	case 8:
		evaluate_curent_line_make_next_line_accessible_to_user (& (widget_ptr->row9), & (widget_ptr->row10), &status);
		break;
	case 9:
		evaluate_curent_line_make_next_line_accessible_to_user (& (widget_ptr->row10), & (widget_ptr->row11), &status);
		break;
	case 10:
		evaluate_curent_line_make_next_line_accessible_to_user (& (widget_ptr->row11), & (widget_ptr->row12), &status);
		break;
	case 11:
		evaluate_curent_line_make_next_line_accessible_to_user (& (widget_ptr->row12), NULL, &status);
		break;
	default:
		exit (EXIT_FAILURE);
		break;
	}
	g_print ("call counter: %d\n", call_counter);
	if (status == 0) //only increment if status is ok
		call_counter++;
}


//##################################################################################################################
//------------------------------------------------------------------------------------------------------------------
//##################################################################################################################



// callback that is executed when one of the "code" buttons in the body is pressed
void cb_code1_clicked_change_apperance (GtkWidget *button, gpointer user_data)
{
	//(void*) user_data; //->no need to typecast to void -gpointer- is synonym for void*
	static gint button_state = 0; //static variable declaration ->will hold value form one func call to next

/*****callback is invoked from test_button clicked******/
	if(user_data != NULL)
	{
		button_state = 0;
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

#if FS_DEBUG_BUTTON
	gtk_button_set_label (GTK_BUTTON (button), "button 1");
#endif
	button_state++;
	if (4 == button_state)
		button_state = 0;
}

// callback that is executed when one of the "code" buttons in the body is pressed
void cb_code2_clicked_change_apperance (GtkWidget *button, gpointer user_data)
{
	static gint button_state = 0; //static variable declaration ->will hold value form one func call to next

	/*****callback is invoked from test_button clicked******/
		if(user_data != NULL)
		{
			button_state = 1;
			return;
		}
	/***********************************************/

	if (button_state == 1) {
		gtk_button_set_label (GTK_BUTTON (button), "0");
		gtk_widget_set_name (button, "button_state0");
	}
	if (button_state == 1) {
		gtk_button_set_label (GTK_BUTTON (button), "1");
		//by setting the name, we change the button theme via our css file
		gtk_widget_set_name (button, "button_state1");
	}
	if (button_state == 2) {
		gtk_button_set_label (GTK_BUTTON (button), "2");
		gtk_widget_set_name (button, "button_state2");
	}
	if (button_state == 3) {
		gtk_button_set_label (GTK_BUTTON (button), "3");
		gtk_widget_set_name (button, "button_state3");
	}
	if (button_state == 4) {
		gtk_button_set_label (GTK_BUTTON (button), "4");
		gtk_widget_set_name (button, "button_state4");
	}
	if (button_state == 5) {
		gtk_button_set_label (GTK_BUTTON (button), "5");
		gtk_widget_set_name (button, "button_state5");
	}
	if (button_state == 6) {
		gtk_button_set_label (GTK_BUTTON (button), "6");
		gtk_widget_set_name (button, "button_state6");
	}
	if (button_state == 7) {
		gtk_button_set_label (GTK_BUTTON (button), "7");
		gtk_widget_set_name (button, "button_state7");
	}

	#if FS_DEBUG_BUTTON
		gtk_button_set_label (GTK_BUTTON (button), "button 2");
	#endif
	button_state++;
	if (button_state == 8)
		button_state = 1;
}

// callback that is executed when one of the "code" buttons in the body is pressed
void cb_code3_clicked_change_apperance (GtkWidget *button, gpointer user_data)
{
	static gint button_state = 0; //static variable declaration ->will hold value form one func call to next

	/*****callback is invoked from test_button clicked******/
		if(user_data != NULL)
		{
			button_state = 0;
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
	#if FS_DEBUG_BUTTON
		gtk_button_set_label (GTK_BUTTON (button), "button 3");
	#endif
	button_state++;
	if (4 == button_state) //loop around
		button_state = 1;
}

// callback that is executed when one of the "code" buttons in the body is pressed
void cb_code4_clicked_change_apperance (GtkWidget *button, gpointer user_data)
{
	static gint button_state = 0; //static variable declaration ->will hold value form one func call to next

	/*****callback is invoked from test_button clicked******/
		if(user_data != NULL)
		{
			button_state = 0;
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

	#if FS_DEBUG_BUTTON
		gtk_button_set_label (GTK_BUTTON (button), "button 4");
	#endif
	button_state++;
	if (4 == button_state)
		button_state = 1;
}
/** EOF */
