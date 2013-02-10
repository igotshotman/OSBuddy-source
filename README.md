## Introduction

The RuneScape Client for Linux is written to provide Linux users with a simple way to play RuneScape and 
update the Client through a simple and familiar interface (GTK).

It is based on HikariKnight's RuneScape For Unix, which is a set of perl scripts bundled with Wx. I simply
wanted to rewrite his scripts in C and use a more common widget set for the interface. (And it was a nice way
to practice my programming in C, which is beginner at best).

Currently, I am in the process of converting the runescape perl script and all its modules to C. Progress is slow
since my experience with C is beginner at best, so all original perl script will still be provided.

The "RuneScape" perl script is written by HikariKnight, but is based on Ethoxyethaan's shellscript to run the RuneScape Windows client natively. For more information, see HikariKnight's website: http://hkprojects.weebly.com

RuneScape is a registred trademark of Jagex Inc.

## Installation

To install RuneScape Client for Linux, you will need the following dependencies:
* java-runtime
* perl
* libcurl
* p7zip
* gtk3
* webkitgtk3

To be able to compile Runescape Client for Linux, you also need the GTK3 development packages. On Ubuntu, this is `libgtk-3-dev`, on Fedora it is `gtk3-devel`. Debian users need at least Debian Wheezy (testing); the GTK3 development libraries are called `libgtk-3-dev`.

When all dependencies are installed, simply run the following commands to install every component:

`make runescape`

`make runescape-launcher`

`make runescape-update-client`

`make install`

To uninstall the RuneScape Client, simply run `make uninstall`.

## ToDo
* improve makefile
* improve error handling
* runescape.c
	* ~~continue with application after running runescape-update-client~~
		* code looks like it can be improved
	* ~~implement skipping of commented lines, instead of making variable (null)~~
		* again, code looks like it can be improved
	* ~~proper language support (currently through runescape.prm) from jagexappletviewer.preferences~~
	* check which java is installed (openjdk/oracle)
		* implement aoss wrapper when java = oracle
	* implement opengl fix
	* implement check for -client mode java
* runescape-launcher.c
	* ~~about dialog icon doesn't show~~
* runescape-update-client.c
	* ~~"Update client" button should stay pressed when downloading~~
	* ~~about dialog icon doesn't show~~
	* ~~get rid of `popen` and `system`: replace with glib alternatives~~
	* ~~actually, the whole `updatefromwindowsclient` function needs check for glib alternatives~~
		* there are none; at least definetely not for popen and fgets. system and strlen do just what we need, instead of the more complicated g_spawn_sync and g_strlcpy 
	* when cancel button is clicked during download, cancel the download (and close the window)
	* at `res != CURLE_OK`: exit when press OK on error dialog -> remove exit(EXIT_FAILURE)
	* before downloading `runescape.msi`, check if it is the complete file;
		* if not, remove it and start download;
		* else, skip download (already implemented)
