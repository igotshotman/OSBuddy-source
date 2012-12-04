mkdir -p /opt/runescape

install:
	install -m 0755 runescape $(DESTDIR)/opt/runescape/runescape
	install -m 0755 runescape-update-client $(DESTDIR)/opt/runescape/runescape-update-client

	install -m 0644 launchers/runescape.desktop $(DESTDIR)/usr/share/applications/runescape.desktop
	install -m 0644 launchers/runescape-update-client.desktop $(DESTDIR)/usr/share/applications/runescape-update-client.desktop
	install -m 0644 launchers/icons/runescape.png $(DESTDIR)/usr/share/icons/hicolor/scalable/apps/runescape.png
	install -m 0644 launchers/icons/runescape-update-client.png $(DESTDIR)/usr/share/icons/hicolor/scalable/runescape-update-client.png

	install -m 0755 modules/rsu_IO.pm $(DESTDIR)/opt/runescape/modules/rsu_IO.pm
	install -m 0755 modules/rsu_check_for_jagexappletviewer.pm $(DESTDIR)/opt/runescape/modules/rsu_check_for_jagexappletviewer.pm
    install -m 0755 modules/rsu_java.pm $(DESTDIR)/opt/runescape/modules/rsu_java.pm
    install -m 0755 modules/rsu_javalib_opengl.pm $(DESTDIR)/opt/runescape/modules/rsu_javalib_opengl.pm
    install -m 0755 modules/rsu_language_support.pm $(DESTDIR)/opt/runescape/modules/rsu_language_support.pm
    install -m 0755 modules/rsu_mains.pm $(DESTDIR)/opt/runescape/modules/rsu_mains.pm
    install -m 0755 modules/rsu_modules.pm $(DESTDIR)/opt/runescape/modules/rsu_modules.pm
    install -m 0755 modules/rsu_prm_filehandler.pm $(DESTDIR)/opt/runescape/modules/rsu_prm_filehandler.pm
    install -m 0755 modules/rsu_verbose.pm $(DESTDIR)/opt/runescape/modules/rsu_verbose.pm

	install -m 0644 settings/runescape.prm $(DESTDIR)/opt/runescape/settings/runescape.prm
	install -m 0644 settings/settings.conf $(DESTDIR)/opt/runescape/settings/settings.conf
