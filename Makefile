install:
	mkdir -p $(DESTDIR)/opt/runescape/modules/
	mkdir -p $(DESTDIR)/opt/runescape/settings/
	mkdir -p $(DESTDIR)/usr/share/applications/
	mkdir -p $(DESTDIR)/usr/share/icons/hicolor/scalable/apps/

	install -m 0755 runescape $(DESTDIR)/opt/runescape/
	install -m 0755 runescape-update-client $(DESTDIR)/opt/runescape/

	install -m 0644 launchers/runescape.desktop $(DESTDIR)/usr/share/applications/
	install -m 0644 launchers/runescape-update-client.desktop $(DESTDIR)/usr/share/applications/
	install -m 0644 launchers/icons/runescape.png $(DESTDIR)/usr/share/icons/hicolor/scalable/apps/
	install -m 0644 launchers/icons/runescape-update-client.png $(DESTDIR)/usr/share/icons/hicolor/scalable/apps/

	install -m 0755 modules/rsu_IO.pm $(DESTDIR)/opt/runescape/modules/
	install -m 0755 modules/rsu_check_for_jagexappletviewer.pm $(DESTDIR)/opt/runescape/modules/
	install -m 0755 modules/rsu_java.pm $(DESTDIR)/opt/runescape/modules/
	install -m 0755 modules/rsu_javalib_opengl.pm $(DESTDIR)/opt/runescape/modules/
	install -m 0755 modules/rsu_language_support.pm $(DESTDIR)/opt/runescape/modules/
	install -m 0755 modules/rsu_mains.pm $(DESTDIR)/opt/runescape/modules/
	install -m 0755 modules/rsu_modules.pm $(DESTDIR)/opt/runescape/modules/
	install -m 0755 modules/rsu_prm_filehandler.pm $(DESTDIR)/opt/runescape/modules/
	install -m 0755 modules/rsu_verbose.pm $(DESTDIR)/opt/runescape/modules/

	install -m 0644 settings/runescape.prm $(DESTDIR)/opt/runescape/settings/
	install -m 0644 settings/settings.conf $(DESTDIR)/opt/runescape/settings/
