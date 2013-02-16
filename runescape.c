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
#include <sys/utsname.h> /*uname*/

static gboolean debug = FALSE;
static gboolean verbose = FALSE;
static gboolean jni = FALSE;
static gboolean gc = FALSE;
static gboolean class = FALSE;
static gboolean client_mode = FALSE;

static GOptionEntry entries[] =
{
	{ "debug", 'd', 0, G_OPTION_ARG_NONE, &debug, "Turn on debug information", NULL },
	{ "verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "Turn on all Java verbose output, equals all of the below combined", NULL },
	{ "verbose:jni", 'j', 0, G_OPTION_ARG_NONE, &jni, "Java: report information about use of native methods and other Java Native Interface activity", NULL },
	{ "verbose:gc", 'g', 0, G_OPTION_ARG_NONE, &gc, "Java: report on each garbage collection event", NULL },
	{ "verbose:class", 'c', 0, G_OPTION_ARG_NONE, &class, "Java: display information about each class loaded", NULL },
	{ NULL }
};

gchar *runescape_bin_dir, *runescape_settings_file, *appletviewer;
gchar *url, *world, *language, *ram, *stacksize, *forcepulseaudio = "false", *forcealsa = "false";
gchar *httpph = NULL, *httppp = NULL, *httpsph = NULL, *httpspp = NULL, *sph = NULL, *spp = NULL;

void
setupfiles(void) {
	const gchar *system_config_dir;
	const gchar* const *system_config_dirs;
	gchar *runescape_config_dir, *installed_settings_file, *runescape_update_client[2];
	gint i;
	GError *error_spawn = NULL;

	runescape_config_dir = g_build_filename(g_get_user_config_dir(), "runescape", NULL);
	runescape_bin_dir = g_build_filename (runescape_config_dir, "bin", NULL);
	runescape_settings_file = g_build_filename (runescape_config_dir, "settings.conf", NULL);
	appletviewer = g_build_filename (g_get_home_dir(), "jagexappletviewer.preferences", NULL);

	if(debug)
		g_fprintf(stdout, "Runescape config directory: %s\nRunescape bin directory: %s\nRunescape settings file: %s\n\n", runescape_config_dir, runescape_bin_dir, runescape_settings_file);

	system_config_dirs = g_get_system_config_dirs ();
	for (i = 0; system_config_dirs[i]; i++) {
		system_config_dir = g_build_filename (system_config_dirs[i], "runescape", NULL);
		if (g_file_test (system_config_dir, G_FILE_TEST_IS_DIR)) {
			installed_settings_file = g_build_filename (system_config_dir, "settings.conf", NULL);
			if(debug)
				g_fprintf(stdout, "System config directory: %s\nInstalled settings file: %s\n\n", system_config_dir, installed_settings_file);
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

	if(g_file_test (runescape_bin_dir, G_FILE_TEST_EXISTS) == FALSE) {
		g_fprintf(stderr, "Could not find %s/jagexappletviewer.jar. We will run runescape-update-client now so it will be installed.\n", runescape_bin_dir);
		runescape_update_client[0] = g_find_program_in_path("runescape-update-client");
		runescape_update_client[1] = NULL;
		g_spawn_sync(NULL, runescape_update_client, NULL, 0, NULL, NULL, NULL, NULL, NULL, &error_spawn);
		if (error_spawn) {
			g_fprintf (stderr, "%s\n", error_spawn->message);
			exit (EXIT_FAILURE);
		}
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
	if(g_key_file_load_from_file (settings, runescape_settings_file, G_KEY_FILE_NONE, NULL) == FALSE)
		g_fprintf(stderr, "Unable to read any settings.conf file. Please check if the RuneScape Client is installed properly. Falling back to defaults.\n\n");
	else {
		world = g_key_file_get_string (settings, "Runescape", "world", NULL);
		ram = g_key_file_get_string (settings, "Java", "ram", NULL);
		stacksize = g_key_file_get_string (settings, "Java", "stacksize", NULL);
		forcepulseaudio = g_key_file_get_string (settings, "Fixes", "forcepulseaudio", NULL);
		forcealsa = g_key_file_get_string (settings, "Fixes", "forcealsa", NULL);
		if(g_key_file_has_group (settings, "Proxy") == TRUE) {
			httpph = g_key_file_get_string (settings, "Proxy", "httpph", NULL);
			httppp = g_key_file_get_string (settings, "Proxy", "httppp", NULL);
			httpsph = g_key_file_get_string (settings, "Proxy", "httpsph", NULL);
			httpspp = g_key_file_get_string (settings, "Proxy", "httpspp", NULL);
			sph = g_key_file_get_string (settings, "Proxy", "sph", NULL);
			spp = g_key_file_get_string (settings, "Proxy", "spp", NULL);
		}
		g_key_file_free(settings);
	}
}

int
main(int argc, char *argv[]) {
	GError *error_parsearg = NULL;
	GOptionContext *context;
	FILE *ldd_output;
	gchar *launchcommand, *java_binary, *ldd_command, opengl_fix[100], *ld_library_path;
	gint java_type;
	struct utsname my_uname;

	context = g_option_context_new ("- launch the RuneScape Client");
	g_option_context_add_main_entries (context, entries, NULL);
	g_option_context_add_group (context, gtk_get_option_group (TRUE));
	if (!g_option_context_parse (context, &argc, &argv, &error_parsearg)) {
		g_fprintf (stderr, "%s\n", error_parsearg->message);
		exit (EXIT_FAILURE);
	}

	setupfiles();
	parselanguage(appletviewer);
	parsesettingsfile(runescape_settings_file);

	if(world)
		url = g_strdup_printf("-Dcom.jagex.config=http://%s.runescape.com/k=3/l=%c/jav_config.ws", world, language[9]);
	else
		url = g_strdup_printf("-Dcom.jagex.config=http://www.runescape.com/k=3/l=%c/jav_config.ws", language[9]);

	java_binary = g_find_program_in_path("java");
	if(g_file_test(java_binary, G_FILE_TEST_IS_SYMLINK) == TRUE) {
		java_binary = g_file_read_link (java_binary, NULL);
		if(g_strrstr(java_binary, "openjdk") != NULL)
			java_type = '1';
		else
			java_type = '0';
	} else {
		if(g_strrstr(java_binary, "openjdk") != NULL)
			java_type = '1';
		else
			java_type = '0';
	}

	ldd_command = g_strjoin(" ", "ldd", java_binary, "| awk '/libjli.so/ {print $3}' | sed 's/jli\\/libjli\\.so//'", NULL);
	ldd_output = popen(ldd_command, "r");
	if (ldd_output != NULL) {
		fgets(opengl_fix, sizeof(opengl_fix), ldd_output);
		pclose(ldd_output);
		opengl_fix[strlen(opengl_fix)-1] = '\0';
		ld_library_path = g_strjoin("", "LD_LIBRARY_PATH=", opengl_fix, NULL);
	} else
		g_fprintf(stderr, "Could not retrieve the path to libjli.so: Java will run without OpenGL implementation\n\n");

	if(uname(&my_uname) == -1) {
		if(debug)
			g_fprintf(stdout, "Can not determine OS data type. We will assume java -client mode is not available\n\n");
	} else {
		if(g_strcmp0(my_uname.machine, "i386") == 0 || g_strcmp0(my_uname.machine, "i586") == 0 || g_strcmp0(my_uname.machine, "i686") == 0) {
			client_mode = TRUE;
			if(debug)
				g_fprintf(stdout, "Data type supports java -client mode!\n\n");
		} else {
			if(debug)
				g_fprintf(stdout, "Data type does not support java -client mode\n\n");
		}
	}

	if(debug) {
		g_fprintf(stdout, "Java binary: %s\nJava type: %c\nLD_LIBRARY_PATH: %sWorld: %s\nLanguage: %c\nRam: %s\nStacksize: %s\nPulseaudio: %s\nAlsa: %s\n\n", java_binary, java_type, opengl_fix, world, language[9], ram, stacksize, forcepulseaudio, forcealsa);
		if(httpph && httppp && httpsph && httpspp)
			g_fprintf(stdout, "httpph: %s\nhttppp: %s\nhttpsph: %s\nhttpspp: %s\n\n", httpph, httppp, httpsph, httpspp);
		if(sph && spp)
			g_fprintf(stdout, "sph: %s\nspp: %s\n\n", sph, spp);
	}

	if(client_mode)
		launchcommand = g_strjoin(" ", java_binary, "-client", "-cp jagexappletviewer.jar", url, NULL);
	else
		launchcommand = g_strjoin(" ", java_binary, "-cp jagexappletviewer.jar", url, NULL);
	if(g_strcmp0(forcepulseaudio, "true") == 0 && g_strcmp0(forcealsa, "true") == 0) {
		g_fprintf(stderr, "Can't use both alsa and pulseaudio! Please disable one or the other. Exiting.\n");
		exit (EXIT_FAILURE);
	} else if(g_strcmp0(forcealsa, "true") == 0 && g_strcmp0(forcepulseaudio, "false") == 0) {
		if (java_type == '1')
			launchcommand = g_strjoin(" ", launchcommand, "-Djavax.sound.sampled.Clip=com.sun.media.sound.DirectAudioDeviceProvider", "-Djavax.sound.sampled.Port=com.sun.media.sound.PortMixerProvider", "-Djavax.sound.sampled.SourceDataLine=com.sun.media.sound.DirectAudioDeviceProvider", "-Djavax.sound.sampled.TargetDataLine=com.sun.media.sound.DirectAudioDeviceProvider", NULL);
		else if(java_type == '0')
			launchcommand = g_strjoin(" ", "aoss", launchcommand, NULL);
	} else if(g_strcmp0(forcealsa, "false") == 0 && g_strcmp0(forcepulseaudio, "true") == 0)
		launchcommand = g_strjoin(" ", "padsp", launchcommand, NULL);
	if(ld_library_path)
		launchcommand = g_strjoin(" ", ld_library_path, launchcommand, NULL);
	if(ram)
		launchcommand = g_strjoin(" ", launchcommand, ram, NULL);
	if(stacksize)
		launchcommand = g_strjoin(" ", launchcommand, stacksize, NULL);
	if(httpph && httppp && httpsph && httpspp)
		launchcommand = g_strjoin(" ", launchcommand, httpph, httppp, httpsph, httpspp, NULL);
	if(sph && spp)
		launchcommand = g_strjoin(" ", launchcommand, sph, spp, NULL);
	if(debug)
		launchcommand = g_strjoin(" ", launchcommand, "-Xdebug", NULL);
	if(verbose)
		launchcommand = g_strjoin(" ", launchcommand, "-verbose", NULL);
	if(jni)
		launchcommand = g_strjoin(" ", launchcommand, "-verbose:jni", NULL);
	if(gc)
		launchcommand = g_strjoin(" ", launchcommand, "-verbose:gc", NULL);
	if(class)
		launchcommand = g_strjoin(" ", launchcommand, "-verbose:class", NULL);
	launchcommand = g_strjoin(" ", launchcommand, "jagexappletviewer /share", NULL);

	g_free(url);
	if(world)
		g_free(world);
	if(g_file_test (appletviewer, G_FILE_TEST_EXISTS) == TRUE)
		g_free(language);
	if(ram)
		g_free(ram);
	if(stacksize)
		g_free(stacksize);
	if(g_file_test (runescape_settings_file, G_FILE_TEST_EXISTS) == TRUE) {
		g_free(forcepulseaudio);
		g_free(forcealsa);
	}
	if(httpph)
		g_free(httpph);
	if(httppp)
		g_free(httppp);
	if(httpsph)
		g_free(httpsph);
	if(httpspp)
		g_free(httpspp);
	if(sph)
		g_free(sph);
	if(spp)
		g_free(spp);
	if(java_binary)
		g_free(java_binary);
	if(ld_library_path)
		g_free(ld_library_path);

	if(debug) {
		g_fprintf(stdout, "Launch command:\n%s\n\n", launchcommand);
		g_fprintf(stdout, "You are now in the hands of Jagex. Good luck on your RuneScape adventures!\n\n");
	}

	g_chdir(runescape_bin_dir);
	system(launchcommand);
	g_free(launchcommand);

	return EXIT_SUCCESS;
}

