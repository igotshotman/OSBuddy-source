runescape-launcher: runescape-launcher.c
    cc -Wall -pedantic -std=c99 `pkg-config --cflags --libs gtk+-3.0 webkitgtk-3.0` runescape-launcher.c -o runescape-launcher

install:
	mkdir -p $(DESTDIR)/opt/runescape/
	mkdir -p $(DESTDIR)/opt/runescape/modules/
	mkdir -p $(DESTDIR)/opt/runescape/settings/
	mkdir -p $(DESTDIR)/usr/share/applications/
	mkdir -p $(DESTDIR)/usr/share/icons/hicolor/scalable/apps/

	install -m 0755 runescape $(DESTDIR)/opt/runescape/
	install -m 0755 runescale-launcher $(DESTDIR)/opt/runescape/
	install -m 0755 runescape-update-client $(DESTDIR)/opt/runescape/
	install -m 0755 modules/* $(DESTDIR)/opt/runescape/modules/
	install -m 0644 settings/* $(DESTDIR)/opt/runescape/settings
	install -m 0644 launchers/runescape.desktop $(DESTDIR)/usr/share/applications/
	install -m 0644 launchers/runescape-update-client.desktop $(DESTDIR)/usr/share/applications/
	install -m 0644 launchers/icons/runescape.png $(DESTDIR)/usr/share/icons/hicolor/scalable/apps/
	install -m 0644 launchers/icons/runescape-update-client.png $(DESTDIR)/usr/share/icons/hicolor/scalable/apps/

clean:
    rm -rf runescape-launcher
