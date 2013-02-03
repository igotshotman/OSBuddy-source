## Introduction

The RuneScape Client for Linux is written to provide Linux users with a simple way to play RuneScape and 
update the Client through a simple and familiar interface (GTK).

It is based on HikariKnight's RuneScape For Unix, which is a set of perl scripts bundled with Wx. I simply
wanted to rewrite his scripts in C and use a more common widget set for the interface. (And it was a nice way
to practice my programming in C, which is beginner at best).

Currently, I am in the process of converting the runescape perl script and all its modules to C. Progress is slow
since my experience with C is beginner at best, so all original perl script will still be provided.

The "RuneScape" perl script is written by HikariKnight, but is based on
Ethoxyethaan's shellscript to run the RuneScape Windows client natively.

For more information, see HikariKnight's website:
http://hkprojects.weebly.com

RuneScape is a registred trademark of Jagex Inc.

## Installation

To install RuneScape Client for Linux, you will need the following dependencies:
* java-runtime
* perl
* libcurl
* p7zip
* gtk3
* webkitgtk3

To be able to compile Runescape Client for Linux, you also need the GTK3 development packages. On Ubuntu, this is `libgtk-3-dev`, on Fedora it is `gtk3-devel`.

When all dependencies are installed, simply run the following commands to install every component:

`make runescape-launcher`

`make runescape-update-client`

`make install`

## ToDo
* improve makefile
* improve error handling
* port the runescape perl script and all its modules to C
* runescape-launcher.c
	* fix the compile messages
* runescape-update-client.c
	* get rid of `popen` and `system`: replace with glib alternatives
	* actually, the whole `updatefromwindowsclient` function needs check for glib alternatives
	* when cancel button is clicked during download, cancel the download (and close the window)
	* at `res != CURLE_OK`: exit when press OK on error dialog -> remove exit(EXIT_FAILURE)
	* before downloading `runescape.msi`, check if it is the complete file;
		* if not, remove it and start download;
		* else, skip download (already implemented)
