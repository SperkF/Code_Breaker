#ifndef _FSfunctions_
#define _FSfunctions_

#include <gtk/gtk.h>
#include <string.h>
#include <libintl.h>
#include "dataTypes.h"

// path/file ->to include files that are not in same directory
// .=current directory; ->then step into includes directory
#define TEXTFILE_GAME_STATS "./includes/game_statistics.txt"
#define TEXTFILE_README "./includes/readme.txt"
#define _(string) gettext (string)

//note: multple functions inside the functions.h are static ->only viable in this file and no other



/*function creates widgets for one line and adds them to grid*/
void code_breaker_add_new_row (GtkWidget *grid, gint row, struct widget_of_one_row *s_current_row, gint key, struct my_widgets *data);
void evaluate_curent_line_make_next_line_accessible_to_user (struct widget_of_one_row *s_current_row, struct widget_of_one_row *s_next_row, gchar *status, struct my_widgets *data);
// callback that is executed when the "test" button in the menu is pressed
void test_button_clicked (GtkWidget *button, gpointer user_data);
void randomize (gchar arr[], int n);
//callbacks for the code buttons
void cb_code1_clicked_change_apperance (GtkWidget *button, gpointer user_data);
void cb_code2_clicked_change_apperance (GtkWidget *button, gpointer user_data);
void cb_code3_clicked_change_apperance (GtkWidget *button, gpointer user_data);
void cb_code4_clicked_change_apperance (GtkWidget *button, gpointer user_data);

//menu callbacks
void submit_button_callback (GSimpleAction *action, GVariant *parameter, gpointer data);
void retry_button_callback (GSimpleAction *action, GVariant *parameter, gpointer data);

void alph_easy_callback (GSimpleAction *action, GVariant *parameter, gpointer data);
void alph_med_callback (GSimpleAction *action, GVariant *parameter, gpointer data);
void alph_hard_callback (GSimpleAction *action, GVariant *parameter, gpointer data);

void tries_easy_callback (GSimpleAction *action, GVariant *parameter, gpointer data);
void tries_med_callback (GSimpleAction *action, GVariant *parameter, gpointer data);
void tries_hard_callback (GSimpleAction *action, GVariant *parameter, gpointer data);

void about_callback (GSimpleAction *action, GVariant *parameter, gpointer data);
void stats_callback (GSimpleAction *action, GVariant *parameter, gpointer data);
gint TimerCallback (gpointer data);
void on_response (GtkDialog *dialog, gint response_id, gpointer user_data);
void on_response_2 (GtkDialog *dialog, gint response_id, gpointer user_data);




#endif
/*EOF*/
