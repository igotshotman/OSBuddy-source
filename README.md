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
* libcurl
* p7zip
* gtk3
* glib
* webkitgtk3

To be able to compile Runescape Client for Linux, you also need development packages. On Arch, you won't need any other packages than the above mentioned. For apt-based distributions, such as Ubuntu and Debian (for Debian, you will need at least Wheezy (testing)), you will need these packages:
* libgtk-3-dev
* libcurl3-dev
* libglib-2.0-dev
* libwebkitgtk-3.0-dev

On rpm-based distributions such as Fedora, you will need these:
* gtk3-devel
* libcurl-devel
* glib-devel
* webkitgtk3-devel

When all dependencies are installed, simply run the following commands to install every component:

`make runescape`

`make runescape-launcher`

`make runescape-update-client`

`make install` (as root)

To uninstall the RuneScape Client, simply run `make uninstall` (as root).

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
	* ~~implement opengl fix~~
		* see if there's a native C way of handling this
	* ~~implement check for -client mode java~~
* runescape-update-client.c
	* ~~"Update client" button should stay pressed when downloading~~
	* ~~about dialog icon doesn't show~~
	* ~~get rid of `popen` and `system`: replace with glib alternatives~~
	* ~~actually, the whole `updatefromwindowsclient` function needs check for glib alternatives~~
		* there are none; at least definetely not for popen and fgets. system and strlen do just what we need, instead of the more complicated g_spawn_sync and g_strlcpy 
	* ~~when cancel button is clicked during download, cancel the download (and close the window)~~
	* ~~at `res != CURLE_OK`: exit when press OK on error dialog -> remove exit(EXIT_FAILURE)~~
		* implemented exit(EXIT_FAILURE) everywhere, the other functions were off. this is proper behaviour
	* ~~before downloading `runescape.msi`, check if it is the complete file;~~
		* ~~if not, remove it and start download;~~
		* ~~else, skip download (already implemented)~~

## Bugs
Should you find any (they are there), please report them to me either by email (see the source files) or open a new Issue on the GitHub page.
