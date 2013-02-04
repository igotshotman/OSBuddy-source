/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * runescape-update-client-gui.c
 * Copyright (C) 2013 Jente <jthidskes@outlook.com>
 *
 * runescape-update-client-gui is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * runescape-update-client-gui is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <unistd.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <glib/gprintf.h>
#include <stdio.h> /*popen*/
#include <stdlib.h> /*system*/
#include <curl/curl.h>

static gboolean debug = FALSE;

static GOptionEntry entries[] =
{
	{ "debug", 'd', 0, G_OPTION_ARG_NONE, &debug, "Turn on debug information", NULL },
	{ NULL }
};

GtkWidget *window, *progressbar;
GError *error_parsearg = NULL, *error_config_dir = NULL, *error_bin_dir = NULL;
gchar *runescape_config_dir, *runescape_bin_dir;

void
getdirs()
{
	const gchar *user_config_dir;
	GDir *config2, *bindir2;
	GtkWidget *error_dialog;

	user_config_dir = g_get_user_config_dir();
	runescape_config_dir = g_build_filename(user_config_dir, "runescape", NULL);
	if (runescape_config_dir != NULL) {
		config2 = g_dir_open(runescape_config_dir, 0, &error_config_dir);
		if (error_config_dir) {
			g_mkdir_with_parents(runescape_config_dir, 0755);
			g_error_free(error_config_dir);
		} else {
			g_dir_close(config2);
		}
	} else {
		error_dialog = gtk_message_dialog_new ((GtkWindow *)window, GTK_DIALOG_DESTROY_WITH_PARENT,
												GTK_MESSAGE_ERROR,
												GTK_BUTTONS_OK,
												"Could not read XDG_CONFIG_HOME\n");
		g_signal_connect (GTK_DIALOG (error_dialog), "response", G_CALLBACK (gtk_main_quit), NULL);
		gtk_dialog_run(GTK_DIALOG(error_dialog));
	}

	runescape_bin_dir = g_build_filename (runescape_config_dir, "bin", NULL);
	bindir2 = g_dir_open(runescape_bin_dir, 0, &error_bin_dir);
	if(error_bin_dir) {
		g_mkdir_with_parents(runescape_bin_dir, 0755);
		g_error_free(error_bin_dir);
	} else {
		g_dir_close(bindir2);
	}

	if(debug) {
		g_fprintf(stdout, "User config directory: %s\n", user_config_dir);
		g_fprintf(stdout, "Runescape config directory: %s\n", runescape_config_dir);
		g_fprintf(stdout, "Runescape bin directory: %s\n\n", runescape_bin_dir);
	}
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}

static int
update_progressbar(GtkWidget *progressbar, double dltotal, double dlnow)
{
	double fraction;
	fraction = dlnow / dltotal;

	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (progressbar), fraction);
	/* allow GTK to refresh the UI */
	while (gtk_events_pending ())
		gtk_main_iteration ();

	return 0;
}

void
downloadwindowsclient()
{
	CURL *curl;
	FILE *fp;
	CURLcode res;
	GtkWidget *error_dialog;

	g_chdir(runescape_bin_dir);

	if(access("runescape.msi", F_OK ) == -1 ) {
		if(debug)
			g_fprintf(stdout, "Did not find runescape.msi, so we will download it now\n\n");
		curl = curl_easy_init();
		if(curl) {
			fp = g_fopen("runescape.msi" , "wb");
			curl_easy_setopt(curl, CURLOPT_URL, "http://www.runescape.com/downloads/runescape.msi");
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

			curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
			curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, update_progressbar);
			curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, progressbar);

			if(debug)
				curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

			res = curl_easy_perform(curl);

			if(res != CURLE_OK) {
				if(debug)
					g_fprintf(stderr, "curl_easy_perform() failed: %s\n\n", curl_easy_strerror(res));
				error_dialog = gtk_message_dialog_new ((GtkWindow *)window, GTK_DIALOG_DESTROY_WITH_PARENT,
														GTK_MESSAGE_ERROR,
														GTK_BUTTONS_OK,
														"Download failed!\n");
				g_signal_connect (GTK_DIALOG (error_dialog), "response", G_CALLBACK (gtk_main_quit), NULL);
				gtk_dialog_run(GTK_DIALOG(error_dialog));
				exit(EXIT_FAILURE);
			}

			curl_easy_cleanup(curl);
			if(fp)
				fclose(fp);
		}
	} else {
		if(debug)
			g_fprintf(stdout, "Found runescape.msi, so we will skip the download\n");
	}
}

void
updatefromwindowsclient()
{
	GtkWidget *error_dialog;

	FILE *jarfile;
	char extract[100];
	static char extractcommand[122] = "7z e -y runescape.msi ";
	const gchar *applet = "jagexappletviewer.jar";

	g_chdir(runescape_bin_dir);
	jarfile = popen("7z l runescape.msi | grep JagexAppletViewerJarFile* | cut -c54-1000", "r");
	if (jarfile == NULL) {
		error_dialog = gtk_message_dialog_new ((GtkWindow *)window, GTK_DIALOG_DESTROY_WITH_PARENT,
												GTK_MESSAGE_ERROR,
												GTK_BUTTONS_OK,
												"Failed to run command, probably because you do not have p7zip installed.\nPlease doulbe check this and try again\n");
		g_signal_connect (GTK_DIALOG (error_dialog), "response", G_CALLBACK (gtk_main_quit), NULL);
		gtk_dialog_run(GTK_DIALOG(error_dialog));
	}

	fgets(extract, sizeof(extract), jarfile);
	extract[strlen(extract)-1] = '\0';
	if(debug)
		g_fprintf(stdout, "Jarfile: %s\n\n", extract);
	pclose(jarfile);
	strcat(extractcommand, extract);
	if(debug)
		g_fprintf(stdout, "We will use this command to extract jagexappletviewer.jar:\n    %s\n\n", extractcommand);
	system(extractcommand);
	g_rename(extract, applet);

	if(access(applet, F_OK ) == -1 ) {
		error_dialog = gtk_message_dialog_new ((GtkWindow *)window, GTK_DIALOG_DESTROY_WITH_PARENT,
												GTK_MESSAGE_ERROR,
												GTK_BUTTONS_OK,
												"Something went wrong; could not read jagexappletviewer.jar!\n");
		g_signal_connect (GTK_DIALOG (error_dialog), "response", G_CALLBACK (gtk_main_quit), NULL);
		gtk_dialog_run(GTK_DIALOG(error_dialog));
	} else {
		if(!debug)
			g_remove("runescape.msi");
		g_remove(extract);
	}
}

static void
update_client (GtkButton* button)
{
	GtkWidget *message_dialog;
	getdirs();
	downloadwindowsclient();
	updatefromwindowsclient();

	message_dialog = gtk_message_dialog_new ((GtkWindow *)window, GTK_DIALOG_MODAL,
											GTK_MESSAGE_INFO,
											GTK_BUTTONS_OK,
											"Done running the update process! You can now launch the RuneScape script to start playing!\n");
	gtk_widget_show_all (message_dialog);
	g_signal_connect (GTK_DIALOG (message_dialog), "response", G_CALLBACK (gtk_main_quit), NULL);
}

static void
dialog_close (GtkWidget *about_dialog)
{
	gtk_widget_destroy (GTK_WIDGET (about_dialog));
}

static void
about_open ()
{
	GtkWidget *about_dialog;

	const gchar *authors[] = {"Unia (Jente)", "HikariKnight", NULL};
	const gchar *license[] = {
		N_("The RuneScape Client is free software: you can redistribute it and/or modify "
		   "it under the terms of the GNU General Public License as published by "
		   "the Free Software Foundation, either version 3 of the License, or "
		   "(at your option) any later version."),
		N_("The RuneScape Client is distributed in the hope that it will be useful "
		   "but WITHOUT ANY WARRANTY; without even the implied warranty of "
		   "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the "
		   "GNU General Public License for more details."),
		N_("You should have received a copy of the GNU General Public License "
		   "along with this program. If not, see <http://www.gnu.org/licenses/>.")
};

	gchar *license_trans;
	license_trans = g_strjoin ("\n\n", _(license[0]), _(license[1]), _(license[2]), NULL);

	about_dialog = gtk_about_dialog_new ();
	gtk_about_dialog_set_program_name (GTK_ABOUT_DIALOG (about_dialog), "RuneScape Client for Linux");
	gtk_about_dialog_set_comments (GTK_ABOUT_DIALOG (about_dialog), "Play RuneScape on Linux"),
	gtk_about_dialog_set_copyright (GTK_ABOUT_DIALOG (about_dialog), "Copyright \xc2\xa9 2013 Unia (Jente)");
	gtk_about_dialog_set_license (GTK_ABOUT_DIALOG (about_dialog), license_trans);
	gtk_about_dialog_set_wrap_license (GTK_ABOUT_DIALOG (about_dialog), TRUE);
	gtk_about_dialog_set_authors (GTK_ABOUT_DIALOG (about_dialog), authors);
	gtk_about_dialog_set_website_label (GTK_ABOUT_DIALOG (about_dialog), "GitHub repository");
	gtk_about_dialog_set_website (GTK_ABOUT_DIALOG (about_dialog), "https://github.com/Unia/runescape-client");
	gtk_about_dialog_set_logo_icon_name (GTK_ABOUT_DIALOG (about_dialog), "runescape");

	g_signal_connect (GTK_DIALOG (about_dialog), "response", G_CALLBACK (dialog_close), NULL);

	gtk_widget_show (about_dialog);
}

static GtkWidget*
create_window (void)
{
	GtkWidget *label_main, *box_all, *box_button, *box_progressbar, *button_update, *button_cancel, *button_about;

	/* Set up the TOPLEVEL WINDOW */
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	/*gtk_window_set_default_size (GTK_WINDOW (window), 480, 220);*/
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_resizable (GTK_WINDOW(window), FALSE);
	gtk_window_set_title (GTK_WINDOW (window), "RuneScape Update Client");
	gtk_window_set_default_icon_name ("runescape-update-client");
	/* Exit when the window is closed */
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	/* Create a label */
	label_main = gtk_label_new (NULL);
	gtk_label_set_text (GTK_LABEL (label_main), "Due to legal reasons the file jagexappletviewer.jar is not available and/or downloadable in certain countries.\nFor this script to work you must be able to download the official RuneScape client for extraction!\n\nAre you sure you want to update jagexappletviewer.jar (from Jagex) by using the official Windows client?\n");
	gtk_label_set_justify (GTK_LABEL (label_main), GTK_JUSTIFY_CENTER);
	gtk_label_set_line_wrap (GTK_LABEL (label_main), FALSE);

	/* Create a box to hold label, box_progressbar and box_button */
	box_all = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
	/* Create a box to hold all the buttons */
	box_button = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 5);
	/* Create a box to hold the progressbar */
	box_progressbar = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 5);

	/* Create a progressbar */
	progressbar = gtk_progress_bar_new();
	gtk_progress_bar_set_show_text (GTK_PROGRESS_BAR (progressbar), FALSE);
	gtk_progress_bar_set_text (GTK_PROGRESS_BAR (progressbar), FALSE);
	/* Add the progressbar to box_progressbar */
	gtk_box_pack_start (GTK_BOX (box_progressbar), progressbar, TRUE, TRUE, 0);

	/* Create about button */
	button_about = gtk_button_new_with_label (_("About"));
	/* Create update button */
	button_update = gtk_button_new_with_label (_("Update client"));
	/* Create play button */
	button_cancel = gtk_button_new_with_label (_("Cancel"));
	/* Add all buttons to box_button in this order, so they appear from left to right properly*/
	gtk_box_pack_start (GTK_BOX (box_button), button_about, FALSE, FALSE, 0);
	gtk_box_pack_end (GTK_BOX (box_button), button_update, FALSE, FALSE, 0);
	gtk_box_pack_end (GTK_BOX (box_button), button_cancel, FALSE, FALSE, 0);
	/* Update the client when clicking button_update */
	g_signal_connect (button_update, "clicked", G_CALLBACK (update_client), NULL);
	/* Launch RuneScape client when clicking button_play */
	g_signal_connect (button_cancel, "clicked", G_CALLBACK (gtk_main_quit), NULL);
	/* Open About dialog when clicking button_about */
	g_signal_connect (button_about, "clicked", G_CALLBACK (about_open), NULL);

	/* Add label to box_all first, so it appears on top */
	gtk_container_add (GTK_CONTAINER (box_all), label_main);
	/* Add box_progressbar to box_all second, so it appears in the middle */
	gtk_container_add (GTK_CONTAINER (box_all), box_progressbar);
	/* Add box_buttons to box_all last, so they appear on the bottom */
	gtk_container_add (GTK_CONTAINER (box_all), box_button);
	/* Add box_all to TOPLEVEL WINDOW */
	gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET (box_all));
	gtk_container_set_border_width (GTK_CONTAINER (window), 5);

	return window;
}

int
main (int argc, char *argv[])
{
	GOptionContext *context;

	context = g_option_context_new ("- update the RuneScape Client");
	g_option_context_add_main_entries (context, entries, NULL);
	g_option_context_add_group (context, gtk_get_option_group (TRUE));
	if (!g_option_context_parse (context, &argc, &argv, &error_parsearg))
	{
		g_fprintf (stderr, "%s\n", error_parsearg->message);
		exit (EXIT_FAILURE);
	}

	gtk_init (&argc, &argv);
	window = create_window ();
	gtk_widget_show_all (window);
	gtk_main ();
	return 0;
}
