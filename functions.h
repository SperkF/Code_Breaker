#ifndef _FSfunctions_
#define _FSfunctions_

#include <gtk/gtk.h>
#include <string.h>
#include "dataTypes.h"


/*function creates widgets for one line and adds them to grid*/
void code_breaker_add_new_row(GtkWidget *grid, gint row, struct widget_of_one_row *s_current_row, gint key);
void evaluate_curent_line_make_next_line_accessible_to_user (struct widget_of_one_row *s_current_row, struct widget_of_one_row *s_next_row, gchar *status);
// callback that is executed when the "test" button in the menu is pressed
void test_button_clicked (GtkWidget *button, gpointer user_data);
void cb_code1_clicked_change_apperance (GtkWidget *button, gpointer user_data);
void cb_code2_clicked_change_apperance (GtkWidget *button, gpointer user_data);
void cb_code3_clicked_change_apperance (GtkWidget *button, gpointer user_data);
void cb_code4_clicked_change_apperance (GtkWidget *button, gpointer user_data);


#endif
/*EOF*/
