install:
	mkdir -p $(DESTDIR)/opt/runescape/modules/
	mkdir -p $(DESTDIR)/opt/runescape/settings/
	mkdir -p $(DESTDIR)/usr/share/applications/
	mkdir -p $(DESTDIR)/usr/share/icons/hicolor/scalable/apps/

	install -m 0755 runescape $(DESTDIR)/opt/runescape/
	install -m 0755 runescape-update-client $(DESTDIR)/opt/runescape/
	install -D 0755 modules/ $(DESTDIR)/opt/runescape/
	install -D 0644 settings/ $(DESTDIR)/opt/runescape/
	install -m 0644 launchers/runescape.desktop $(DESTDIR)/usr/share/applications/
	install -m 0644 launchers/runescape-update-client.desktop $(DESTDIR)/usr/share/applications/
	install -m 0644 launchers/icons/runescape.png $(DESTDIR)/usr/share/icons/hicolor/scalable/apps/
	install -m 0644 launchers/icons/runescape-update-client.png $(DESTDIR)/usr/share/icons/hicolor/scalable/apps/

