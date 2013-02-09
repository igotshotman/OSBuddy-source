/* gcc -Wall -pedantic runescape.c -o runescapetest `pkg-config --cflags --libs gtk+-3.0` */
/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * runescape.c
 * Copyright (C) 2013 Jente <jthidskes@outlook.com>
 *
 * runescape.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * runescape.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <glib/gstdio.h>
#include <glib/gprintf.h>

#include <stdlib.h> /*system*/
#include <stdio.h> /*popen*/
#include <unistd.h> /*execl*/

static gboolean debug = FALSE;
static gboolean verbose = FALSE;
static gboolean jni = FALSE;
static gboolean gc = FALSE;
static gboolean class = FALSE;

static GOptionEntry entries[] =
{
	{ "debug", 'd', 0, G_OPTION_ARG_NONE, &debug, "Turn on debug information", NULL },
	{ "verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "Turn on all Java verbose output, equals all of the below combined", NULL },
	{ "verbose:jni", 'j', 0, G_OPTION_ARG_NONE, &jni, "Java: report information about use of native methods and other Java Native Interface activity", NULL },
	{ "verbose:gc", 'g', 0, G_OPTION_ARG_NONE, &gc, "Java: report on each garbage collection event", NULL },
	{ "verbose:class", 'c', 0, G_OPTION_ARG_NONE, &class, "Java: display information about each class loaded", NULL },
	{ NULL }
};

gchar *runescape_bin_dir, *runescape_settings_file, *runescape_prm_file, *appletviewer;
gchar *url;
gchar *world;
gchar *language = NULL;
gchar *ram = "-Xmx1024m";
gchar *stacksize= "-Xss1m";
gchar *forcepulseaudio = "false";
gchar *forcealsa = "false";

void
setupfiles(void) {
	const gchar *system_config_dir;
	const gchar* const *system_config_dirs;
	gchar *runescape_config_dir, *runescape_settings_dir;
	gchar *installed_settings_file, *installed_prm_file;
	gint i;
	GError *error_spawn = NULL;
	gchar *argv[2];

	runescape_config_dir = g_build_filename(g_get_user_config_dir(), "runescape", NULL);
	runescape_bin_dir = g_build_filename (runescape_config_dir, "bin", NULL);
	runescape_settings_dir = g_build_filename (runescape_config_dir, "settings", NULL);

	runescape_settings_file = g_build_filename (runescape_settings_dir, "settings.conf", NULL);
	runescape_prm_file = g_build_filename (runescape_settings_dir, "runescape.prm", NULL);
	appletviewer = g_build_filename (g_get_home_dir(), "jagexappletviewer.preferences", NULL);

	if(debug) {
		g_fprintf(stdout, "Runescape config directory: %s\n", runescape_config_dir);
		g_fprintf(stdout, "Runescape bin directory: %s\n", runescape_bin_dir);
		g_fprintf(stdout, "Runescape settings directory: %s\n", runescape_settings_dir);
		g_fprintf(stdout, "Runescape settings file: %s\n", runescape_settings_file);
		g_fprintf(stdout, "Runescape prm file: %s\n\n", runescape_prm_file);
	}

	system_config_dirs = g_get_system_config_dirs ();
	for (i = 0; system_config_dirs[i]; i++) {
		system_config_dir = g_build_filename (system_config_dirs[i], "runescape", NULL);
		if (g_file_test (system_config_dir, G_FILE_TEST_IS_DIR)) {
			installed_settings_file = g_build_filename (system_config_dir, "settings", "settings.conf", NULL);
			installed_prm_file = g_build_filename (system_config_dir, "settings", "runescape.prm", NULL);
			if(debug) {
				g_fprintf(stdout, "System config directory: %s\n", system_config_dir);
				g_fprintf(stdout, "Installed settings file: %s\n", installed_settings_file);
				g_fprintf(stdout, "Installed prm file: %s\n\n", installed_prm_file);
			}
		} else {
			if(debug)
				g_fprintf(stdout, "Could not retrieve any system config directories\n\n");
		}
		g_free ((gpointer)system_config_dir);
	}

	if(g_file_test (runescape_settings_file, G_FILE_TEST_EXISTS) == FALSE) {
		if(g_file_test (installed_settings_file, G_FILE_TEST_EXISTS) == TRUE) {
			if(debug)
				g_fprintf(stdout, "Did not find %s, so we will use the system installed settings file.\n", runescape_settings_file);
			runescape_settings_file = installed_settings_file;
		}
	}
	if(g_file_test (runescape_prm_file, G_FILE_TEST_EXISTS) == FALSE) {
		if(g_file_test (installed_prm_file, G_FILE_TEST_EXISTS) == TRUE) {
			if(debug)
				g_fprintf(stdout, "Did not find %s, so we will use the system installed prm file.\n\n", runescape_prm_file);
			runescape_prm_file = installed_prm_file;
		}
	}

	if(g_file_test (runescape_bin_dir, G_FILE_TEST_EXISTS) == FALSE) {
		g_fprintf(stderr, "Could not find %s/jagexappletviewer.jar. We will run runescape-update-client now so it will be installed.\n", runescape_bin_dir);
		argv[0] = g_find_program_in_path("runescape-update-client");
		argv[1] = NULL;
		g_spawn_sync(NULL, argv, NULL, G_SPAWN_SEARCH_PATH_FROM_ENVP, NULL, NULL, NULL, NULL, NULL, &error_spawn);
		if (error_spawn) {
			g_fprintf (stderr, "%s\n", error_spawn->message);
			exit (EXIT_FAILURE);
		}
	}
}

void
parseprmfile(gchar *runescape_prm_file) {
	GKeyFile *prm;

	prm = g_key_file_new();
	if(g_key_file_load_from_file (prm, runescape_prm_file, G_KEY_FILE_NONE, NULL) == FALSE) {
		g_fprintf(stderr, "Unable to read any runescape.prm file. Please check if the RuneScape Client is installed properly. Falling back to defaults.\n\n");
	} else {
		world = g_key_file_get_string (prm, "Runescape", "world", NULL);
		ram = g_key_file_get_string (prm, "Java", "ram", NULL);
		stacksize = g_key_file_get_string (prm, "Java", "stacksize", NULL);
		g_key_file_free(prm);
	}
}

void
parselanguage(gchar *appletviewer) {
	if(g_file_get_contents(appletviewer, &language, NULL, NULL) == FALSE) {
		g_fprintf(stderr, "Unable to read file: %s. Falling back to English.\n", appletviewer);
		language = "Language=0";
	}
}

void
parsesettingsfile(gchar *runescape_settings_file) {
	GKeyFile *settings;

	settings = g_key_file_new();
	if(g_key_file_load_from_file (settings, runescape_settings_file, G_KEY_FILE_NONE, NULL) == FALSE) {
		g_fprintf(stderr, "Unable to read any settings.conf file. Please check if the RuneScape Client is installed properly. Falling back to defaults.\n\n");
	} else {
		forcepulseaudio = g_key_file_get_string (settings, "Fixes", "forcepulseaudio", NULL);
		forcealsa = g_key_file_get_string (settings, "Fixes", "forcealsa", NULL);
		g_key_file_free(settings);
	}
}

int
main(int argc, char *argv[]) {
	GError *error_parsearg = NULL;
	GOptionContext *context;
	gchar *launchcommand, *java_binary;

	context = g_option_context_new ("- launch the RuneScape Client");
	g_option_context_add_main_entries (context, entries, NULL);
	g_option_context_add_group (context, gtk_get_option_group (TRUE));
	if (!g_option_context_parse (context, &argc, &argv, &error_parsearg)) {
		g_fprintf (stderr, "%s\n", error_parsearg->message);
		exit (EXIT_FAILURE);
	}

	setupfiles();
	parseprmfile(runescape_prm_file);
	parselanguage(appletviewer);
	parsesettingsfile(runescape_settings_file);

	if(world) {
		url = g_strdup_printf("-Dcom.jagex.config=http://%s.runescape.com/k=3/l=%d/jav_config.ws", world, language[9]-48);
	} else {
		url = g_strdup_printf("-Dcom.jagex.config=http://www.runescape.com/k=3/l=%d/jav_config.ws", language[9]-48);
	}

	java_binary = g_find_program_in_path("java");

	if(debug) {
		g_fprintf(stdout, "World:%s\nLanguage:%d\n", world, language[9]-48);
		g_fprintf(stdout, "Ram: %s\nStacksize: %s\n", ram, stacksize);
		g_fprintf(stdout, "Pulseaudio: %s\nAlsa: %s\n\n", forcepulseaudio, forcealsa);
		g_fprintf(stdout, "Java binary: %s\n\n", java_binary);
	}

	launchcommand = g_strjoin(" ", java_binary, "-cp jagexappletviewer.jar", "-Djava.class.path=jagexappletviewer.jar", url, NULL);
	launchcommand = g_strjoin(" ", launchcommand, ram, NULL);
	launchcommand = g_strjoin(" ", launchcommand, stacksize, NULL);
	if(debug) {
		launchcommand = g_strjoin(" ", launchcommand, "-Xdebug", NULL);
	}
	if(verbose) {
		launchcommand = g_strjoin(" ", launchcommand, "-verbose", NULL);
	}
	if(jni) {
		launchcommand = g_strjoin(" ", launchcommand, "-verbose:jni", NULL);
	}
	if(gc) {
		launchcommand = g_strjoin(" ", launchcommand, "-verbose:gc", NULL);
	}
	if(class) {
		launchcommand = g_strjoin(" ", launchcommand, "-verbose:class", NULL);
	}
	if(g_strcmp0(forcepulseaudio, "true") == 0) {
		if(g_strcmp0(forcealsa, "true") != 0) {
			launchcommand = g_strjoin(" ", "padsp", launchcommand, NULL);
		} else if(g_strcmp0(forcealsa, "true") == 0) {
			g_fprintf(stderr, "Can't use both alsa and pulseaudio! Please disable one or the other. Exiting.\n");
			exit (EXIT_FAILURE);
		}
	} else if(g_strcmp0(forcealsa, "true") == 0) {
		if(g_strcmp0(forcepulseaudio, "true") != 0) {
			/*if (java_binary = opendjk)*/
				launchcommand = g_strjoin(" ", launchcommand, "-Djavax.sound.sampled.Clip=com.sun.media.sound.DirectAudioDeviceProvider", "-Djavax.sound.sampled.Port=com.sun.media.sound.PortMixerProvider", "-Djavax.sound.sampled.SourceDataLine=com.sun.media.sound.DirectAudioDeviceProvider", "-Djavax.sound.sampled.TargetDataLine=com.sun.media.sound.DirectAudioDeviceProvider", NULL);
			/*else if (java_binay = oracle)
				aoss wrapper*/
		} else if(g_strcmp0(forcepulseaudio, "true") == 0) {
			g_fprintf(stderr, "Can't use both pulseaudio and alsa! Please disable one or the other. Exiting.\n");
			exit (EXIT_FAILURE);
		}
	}
	launchcommand = g_strjoin(" ", launchcommand, "jagexappletviewer /share", NULL);

	g_free(url);
	g_free(world);
	g_free(language);
	g_free(ram);
	g_free(stacksize);
	g_free(forcepulseaudio);
	g_free(forcealsa);
	g_free(java_binary);

	if(debug) {
		g_fprintf(stdout, "Launch command: %s\n\n", launchcommand);
		g_fprintf(stdout, "You are now in the hands of Jagex. Good luck on your RuneScape adventures!\n\n");
	}

	g_chdir(runescape_bin_dir);
	system(launchcommand);

	return EXIT_SUCCESS;
}

