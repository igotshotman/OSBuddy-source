/* gcc -g `pkg-config --cflags --libs libcurl` runescape-update-client.c -o runescape-update-client-test */
#include <stdlib.h> /*getenv, system*/
#include <string.h> /*strlen, strcpy, strcat*/
#include <stdio.h> /*fprintf, popen*/
#include <sys/stat.h> /*opendir, mkdir*/
#include <sys/types.h> /*mkdir*/
#include <dirent.h> /*opendir*/
#include <unistd.h> /*chdir, execl*/
#include <curl/curl.h> /*libcurl*/
#include <errno.h> /*errno*/

char clientdir[FILENAME_MAX], bindir[FILENAME_MAX], oldpath[FILENAME_MAX], newpath[FILENAME_MAX];

struct FtpFile {
	const char *filename;
	FILE *stream;
};

void
getdirs() {
	DIR *clientdir2, *bindir2;

	char *config = getenv("XDG_CONFIG_HOME");
	if (config != NULL) {
		strcat(config, "/runescape");
		strcpy(clientdir, config);
		clientdir2 = opendir(clientdir);
		if(clientdir2 == NULL) {
			mkdir(clientdir, 0755);
		} else {
			closedir(clientdir2);
		};
	} else {
		fprintf(stderr, "Could not get XDG_CONFIG_HOME.\n");
		exit(0);
	};

	strcpy(bindir, clientdir);
	strcat(bindir, "/bin");
	bindir2 = opendir(bindir);
	if(bindir2 == NULL) {
		mkdir(bindir, 0755);
	} else {
		closedir(bindir2);
	};
}

static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream) {
	struct FtpFile *out=(struct FtpFile *)stream;
	if(out && !out->stream) {
		/* open file for writing */
		out->stream=fopen(out->filename, "wb");
		if(!out->stream)
			return -1;
	}
	return fwrite(buffer, size, nmemb, out->stream);
}

void
downloadwindowsclient() {
	CURL *curl;
	CURLcode res;

	struct FtpFile ftpfile = {
    	"runescape.msi",
		NULL
	};

	chdir(bindir);

	if(access("runescape.msi", F_OK ) == -1 ) {
		curl = curl_easy_init();
		if(curl) {
			curl_easy_setopt(curl, CURLOPT_URL, "http://www.runescape.com/downloads/runescape.msi");
			/* Define our callback to get called when there's data to be written */
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
			/* Set a pointer to our struct to pass to the callback */
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);
			/*curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); */
			curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
			res = curl_easy_perform(curl);

			if(res != CURLE_OK) {
				fprintf(stderr, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));
			}

			if(ftpfile.stream)
				fclose(ftpfile.stream);
			curl_easy_cleanup(curl);
		};
	};
}

void
updatefromwindowsclient() {
	FILE *jarfile;
	char extract[100];
	static char extractcommand[122] = "7z e -y runescape.msi ";
	const char *applet = "jagexappletviewer.jar";

	chdir(bindir);
	jarfile = popen("7z l runescape.msi | grep JagexAppletViewerJarFile* | cut -c54-1000", "r");
	if (jarfile == NULL) {
		fprintf(stderr, "Failed to run command, probably because you do not have p7zip installed.\nPlease double check this and try again.\n" );
		exit(0);
	};

	fgets(extract, sizeof(extract), jarfile);
	extract[strlen(extract)-1] = '\0';
	pclose(jarfile);
	strcat(extractcommand, extract);
	system(extractcommand);
	rename(extract, applet));

	if(access(applet, F_OK ) == -1 ) {
		fprintf(stderr, "Something went wrong; could not read jagexappletviewer.jar in bindir!\n");
		exit(0);
	} else {
		remove("runescape.msi");
		remove(extract);
	};
}

int
main() {
	char answer;

	fprintf(stdout, "Due to legal reasons the file jagexappletviewer.jar is not available and/or downloadable in certain countries. For this script to work you must be able to download the official RuneScape client for extraction!\n\nAre you sure you want to update jagexappletviewer.jar (from Jagex) by using the official Windows client?\nAnswer [Y/n]: ");

	scanf("%c", &answer);
	if(answer == 'n' || answer == 'N') {
		exit(0);
	} else if(answer == 'y' || answer == 'Y') {
		getdirs();
		downloadwindowsclient();
		updatefromwindowsclient();
		fprintf(stdout, "Done running the update process! You can now launch the RuneScape script to start playing!\n");
		exit(0);
	};
	return EXIT_SUCCESS;
}

