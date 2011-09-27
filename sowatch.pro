TEMPLATE = subdirs
SUBDIRS = libsowatch \
	metawatch \
	sowatchd \
	sysinfowatchlet

!isEmpty(MEEGO_VERSION_MAJOR) {
	SUBDIRS += meegohandsetnotification ckitcallnotification qmafwwatchlet
}

unix:!symbian {
	maemo5 {
		target.path = /opt/sowatch/notifications
	} else {
		target.path = /usr/lib/sowatch/notifications
	}
	INSTALLS += target
}

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
	qtc_packaging/debian_harmattan/changelog \
	qtc_packaging/debian_harmattan/manifest.aegis













