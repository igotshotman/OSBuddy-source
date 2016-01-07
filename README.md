Play RuneScape on Linux
=======================

These scripts provide you with a simple and easy way to play RuneScape on Linux. The scrips might
work on other operating systems (with minimal changes), but no regards have been made towards
portability. Please note that simple really does mean simple: the `runescape` script does just
enough to launch the JVM with the RuneScape game client for the regular game. Old school, Darkscape
and other game modes are not supported! In case you need more functionality, please see
HikariKnight's [rsu-client][rsu-client].

For convenience, the script parses a configuration file which can be used to automatically start a
certain world and to feed options to the JVM. In-game language settings are also supported by
parsing the `jagexappletviewer.preferences` file.

A nice clean up that is applied is setting Java's `-Duser.home` to the configuration directory used
by this script (`$XDG_CONFIG_HOME/runescape` by default). This results in a cleaner home directory,
because Jagex' files will now end up there instead.

These scripts are based on an early version of the [rsu-client][rsu-client] by HikariKnight.
RuneScape is a registred trademark of Jagex Inc.

Installation
------------

To use the scripts, you will need to have the following dependencies installed:
* python3
* java-runtime
* p7zip
* python-gobject
* webkitgtk

When all these dependencies are installed, simply place the scripts in your `$PATH` and run them.

Usage
-----
An example configuration file can be found [in the settings][config]. You can copy it to
`$XDG_CONFIG_HOME/runescape` (usually `~/.config/runescape`) and edit it to change the behaviour
of the client.

Bugs
----

For any bug or request [fill an issue](https://github.com/Unia/runescape-client/issues) on [GitHub][github].

License
-------

Please see [LICENSE](https://github.com/Unia/runescape-client/blob/master/LICENSE) on [GitHub][github].

**Copyright © 2015** Jente Hidskes &lt;hjdskes@gmail.com&gt;

[github]: https://github.com/Unia/runescape-client
[rsu-client]: https://github.com/HikariKnight/rsu-client
[config]: https://github.com/Unia/runescape-client/blob/master/settings/settings.ini

