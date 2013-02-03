PREFIX ?= /opt/runescape

runescape-launcher: runescape-launcher.c
	gcc runescape-launcher.c -o runescape-launcher -Wall -pedantic -std=c99 `pkg-config --cflags --libs gtk+-3.0 webkitgtk-3.0`

runescape-update-client: runescape-update-client.c
	gcc runescape-update-client.c -o runescape-update-client -Wall -pedantic `pkg-config --cflags --libs gtk+-3.0 libcurl`

install:
	mkdir -p $(DESTDIR)/opt/runescape/
	mkdir -p $(DESTDIR)/opt/runescape/modules/
	mkdir -p $(DESTDIR)/opt/runescape/settings/
	mkdir -p $(DESTDIR)/usr/share/applications/
	mkdir -p $(DESTDIR)/usr/share/icons/hicolor/scalable/apps/

	install -m 0755 runescape $(DESTDIR)/opt/runescape/
	install -m 0755 runescape-launcher $(DESTDIR)/opt/runescape/
	install -m 0755 runescape-update-client $(DESTDIR)/opt/runescape/
	install -m 0755 modules/* $(DESTDIR)/opt/runescape/modules/
	install -m 0644 settings/* $(DESTDIR)/opt/runescape/settings/
	install -m 0644 launchers/runescape.desktop $(DESTDIR)/usr/share/applications/
	install -m 0644 launchers/runescape-launcher.desktop $(DESTDIR)/usr/share/applications/
	install -m 0644 launchers/runescape-update-client.desktop $(DESTDIR)/usr/share/applications/
	install -m 0644 launchers/icons/* $(DESTDIR)/usr/share/icons/hicolor/scalable/apps/

uninstall:
	rm -rf /opt/runescape
	rm -rf /usr/share/applications/runescape.desktop
	rm -rf /usr/share/applications/runescape-launcher.desktop
	rm -rf /usr/share/applications/runescape-update-client.desktop
	rm -rf /usr/share/icons/hicolor/scalable/apps/runescape.png
	rm -rf /usr/share/icons/hicolor/scalable/apps/runescape-update-client.png

clean:
	rm -rf runescape-launcher
	rm -rf runescape-update-client
