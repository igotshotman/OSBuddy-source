CFLAGS = -Wall
#debug: CFLAGS += -O0 -g -pedantic -Wextra
#debug: all

runescape: runescape.c
	gcc runescape.c -o runescape ${CFLAGS} `pkg-config --cflags --libs gtk+-3.0`

runescape-launcher: runescape-launcher.c
	gcc runescape-launcher.c -o runescape-launcher ${CFLAGS} `pkg-config --cflags --libs gtk+-3.0 webkitgtk-3.0`

runescape-update-client: runescape-update-client.c
	gcc runescape-update-client.c -o runescape-update-client ${CFLAGS} `pkg-config --cflags --libs gtk+-3.0 libcurl`

all: runescape runescape-launcher runescape-update-client

install: all
	mkdir -p $(DESTDIR)/etc/xdg/runescape/
	mkdir -p $(DESTDIR)/usr/bin/
	mkdir -p $(DESTDIR)/usr/share/applications/
	mkdir -p $(DESTDIR)/usr/share/icons/hicolor/scalable/apps/

	install -m 0755 runescape $(DESTDIR)/usr/bin/
	install -m 0755 runescape-launcher $(DESTDIR)/usr/bin/
	install -m 0755 runescape-update-client $(DESTDIR)/usr/bin/
	install -m 0644 settings/settings.conf $(DESTDIR)/etc/xdg/runescape/
	install -m 0644 launchers/runescape.desktop $(DESTDIR)/usr/share/applications/
	install -m 0644 launchers/runescape-launcher.desktop $(DESTDIR)/usr/share/applications/
	install -m 0644 launchers/runescape-update-client.desktop $(DESTDIR)/usr/share/applications/
	install -m 0644 launchers/icons/* $(DESTDIR)/usr/share/icons/hicolor/scalable/apps/

uninstall:
	rm -rf /etc/xdg/runescape
	rm -rf /usr/bin/runescape
	rm -rf /usr/bin/runescape-launcher
	rm -rf /usr/bin/runescape-update-client
	rm -rf /usr/share/applications/runescape.desktop
	rm -rf /usr/share/applications/runescape-launcher.desktop
	rm -rf /usr/share/applications/runescape-update-client.desktop
	rm -rf /usr/share/icons/hicolor/scalable/apps/runescape.png
	rm -rf /usr/share/icons/hicolor/scalable/apps/runescape-update-client.png

clean:
	rm -rf runescape
	rm -rf runescape-launcher
	rm -rf runescape-update-client
