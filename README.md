RuneScape Client for Linux
=========

## Introduction

The RuneScape Client for Linux is written to provide Linux users with a simple way to play RuneScape and 
update the Client through a simple and familiar interface (GTK).

It is based on HikariKnight's RuneScape For Unix, which is a set of perl scripts bundled with Wx. I simply
wanted to rewrite his scripts in C and use a more common widget set for the interface (and it was a nice way
to practice my programming in C).

~~Currently, I am in the process of converting the runescape perl script and all its modules to C. Progress is slow
since my experience with C is beginner at best, so all original perl script will still be provided.~~

The "RuneScape" perl script is written by HikariKnight, but is based on Ethoxyethaan's shellscript to run the RuneScape Windows client natively. For more information, see HikariKnight's [website][site].

[site]: http://hkprojects.weebly.com

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

	$ make runescape
	$ make runescape-launcher
	$ make runescape-update-client
	# make clean install

To uninstall the RuneScape Client, simply run `make uninstall` (as root).

## Usage
A small word on the usage, just so you can get the maximum out of the client. When installed, you can run either of three applications as you
would normally run any application. A configuration file is installed in `/etc/xdg/runescape`. You can copy it to `XDG_CONFIG_HOME` (usually `~/.config/`) and edit it to change the behaviour of the client.

	$ mkdir -p ~/.config/runescape
	$ cp /etc/xdg/runescape/settings.conf ~/.config/runescape/settings.conf

## ToDo
* improve makefile
* improve error handling
* runescape.c
	* ~~continue with application after running runescape-update-client~~
		* code looks like it can be improved
	* ~~implement skipping of commented lines, instead of making variable (null)~~
		* again, code looks like it can be improved
	* ~~check which java is installed (openjdk/oracle)~~
		* **Very** basic, needs more testing acros various distributios (32/64 bit as well)
		* ~~implement aoss wrapper when java = oracle~~
			* Some distributions use aoss32; not implemented yet. Needs the testing mentioned above.
	* ~~implement opengl fix~~
		* see if there's a native C way of handling this
* runescape-update-client.c
	* for now, download size is statically set. we should check total size before download, in case filesize changes

## Bugs
Should you find any, please report them to me either by email (see the source files) or [open a new Issue][issue] on the [GitHub page][ghp].

[issue]: https://github.com/Unia/runescape-client/issues 
[ghp]: https://github.com/Unia/runescape-client/
