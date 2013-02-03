/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * runescape-launcher.c
 * Copyright (C) 2013 Jente <jthidskes@outlook.com>
 *
 * runescape-launcher is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * runescape-launcher is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <glib/gstdio.h>
#include <glib/gprintf.h>
#include <webkit/webkit.h>

static gboolean debug = FALSE;

static GOptionEntry entries[] =
{
	{ "debug", 'd', 0, G_OPTION_ARG_NONE, &debug, "Turn on debug information", NULL },
	{ NULL }
};

gchar *installpath;

char *
getcurrentpath()
{
	installpath = g_get_current_dir();
	if(debug)
		g_fprintf(stdout, "current directory: %s\n", installpath);
	return installpath;
}

static void
launch_client (GtkButton* button)
{
	gchar *runescape_launch;

	getcurrentpath();
	runescape_launch = g_build_filename(installpath, "runescape", NULL);
	if(debug)
		g_fprintf(stdout, "runescape_launch: %s\n", runescape_launch);
	execl(runescape_launch, runescape_launch, NULL);
}

static void
update_client (GtkButton* button)
{
	gchar *runescape_update;

	getcurrentpath();
	runescape_update = g_build_filename(installpath, "runescape-update-client", NULL);
	if(debug)
		g_fprintf(stdout, "runescape_update: %s\n", runescape_update);
	execl(runescape_update, runescape_update, NULL);
}

static void
dialog_close (GtkWidget *about_dialog)
{
	gtk_widget_destroy (GTK_WIDGET (about_dialog));
}

static void
dialog_open ()
{
	GtkWidget *about_dialog;

	about_dialog = gtk_about_dialog_new ();

	const gchar *authors[] = {"Unia (Jente)", "HikariKnight", NULL};
	const gchar *license[] = {
		N_("The RuneScape Client Launcher is free software: you can redistribute it and/or modify "
		   "it under the terms of the GNU General Public License as published by "
		   "the Free Software Foundation, either version 3 of the License, or "
		   "(at your option) any later version."),
		N_("The RuneScape Launcher is distributed in the hope that it will be useful "
		   "but WITHOUT ANY WARRANTY; without even the implied warranty of "
		   "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the "
		   "GNU General Public License for more details."),
		N_("You should have received a copy of the GNU General Public License "
		   "along with this program. If not, see <http://www.gnu.org/licenses/>.")
};

	gchar *license_trans;
	license_trans = g_strjoin ("\n\n", _(license[0]), _(license[1]), _(license[2]), NULL);

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
	GtkWidget *window, *box_all, *box_scroll, *box_button, *button_play, *button_update, *button_about, *scroll;

	/* Set up the UI */
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW (window), 800, 600);
	gtk_window_set_title (GTK_WINDOW (window), "RuneScape Launcher");
	gtk_window_set_default_icon_name ("runescape");

	/* Create box container to hold the webkit view and both buttons */
	box_all = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_container_set_border_width (GTK_CONTAINER (box_all), 5);

	/* Create a box to hold both buttons */
	box_button = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
	/* Create play button */
	button_play = gtk_button_new_with_label (_("Play RuneScape"));
	/* Create update button */
	button_update = gtk_button_new_with_label (_("Update client"));
	/* Create about button */
	button_about = gtk_button_new_with_label (_("About"));
	/* Add all buttons to box_button */
	gtk_box_pack_start (GTK_BOX (box_button), button_play, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (box_button), button_update, FALSE, FALSE, 0);
	gtk_box_pack_end (GTK_BOX (box_button), button_about, FALSE, FALSE, 0);

	/* Create a box to hold the scroll container */
	box_scroll = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
	/* Create scroll container to hold the webkit view*/
	scroll = gtk_scrolled_window_new (NULL, NULL);
	g_object_set (scroll, "shadow-type", GTK_SHADOW_IN, NULL);
	/* Create webkit view and point it to runescape.com */
	WebKitWebView *view = WEBKIT_WEB_VIEW(webkit_web_view_new());
	webkit_web_view_load_uri(view, "http://services.runescape.com/m=news/list.ws");
	/* Add VIEW to SCROLL */
	gtk_container_add (GTK_CONTAINER (scroll), GTK_WIDGET(view));
	/* Add scroll to box_scroll */
	gtk_box_pack_start (GTK_BOX (box_scroll), scroll, TRUE, TRUE, 0);

	/* Add box_scroll to box_all first, so it appears left of the buttons */
	gtk_box_pack_start (GTK_BOX (box_all), box_scroll, TRUE, TRUE, 0);
	/* Add box_button to box_all */
	gtk_box_pack_start (GTK_BOX (box_all), box_button, FALSE, FALSE, 0);
	/* Add box_all to TOPLEVEL WINDOW */
	gtk_container_add (GTK_CONTAINER (window), box_all);

	/* Launch RuneScape client when clicking button_play */
	g_signal_connect (button_play, "clicked", G_CALLBACK (launch_client), NULL);
	/* Update the client when clicking button_update */
	g_signal_connect (button_update, "clicked", G_CALLBACK (update_client), NULL);
	/* Open About dialog when clicking button_about */
	g_signal_connect (button_about, "clicked", G_CALLBACK (dialog_open), NULL);
	/* Exit when the window is closed */
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	return window;
}

int
main (int argc, char *argv[])
{
 	GtkWidget *window;
	GError *error_parsearg = NULL;
	GOptionContext *context;

	context = g_option_context_new ("- a launcher to the RuneScape applications");
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
