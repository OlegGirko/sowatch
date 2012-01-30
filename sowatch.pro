TEMPLATE = subdirs
SUBDIRS = libsowatch

SUBDIRS += sowatchd
SUBDIRS += metawatch
SUBDIRS += sowatchui
SUBDIRS += notificationswatchlet sysinfowatchlet
SUBDIRS += qmsgwatchlet qmapwatchlet

sowatchd.depends = libsowatch
metawatch.depends = libsowatch
sowatchui.depends = libsowatch sowatchd
notificationswatchlet.depends = libsowatch
sysinfowatchlet.depends = libsowatch
qmsgwatchlet.depends = libsowatch
qmapwatchlet.depends = libsowatch

contains(MEEGO_EDITION,harmattan) {
	SUBDIRS += meegohandsetnotification ckitcallnotification harmaccuweather
    SUBDIRS += qmafwwatchlet

	meegohandsetnotification.depends = libsowatch
	ckitcallnotification.depends = libsowatch
	harmaccuweather.depends = libsowatch
    qmafwwatchlet.depends = libsowatch
}

OTHER_FILES += \
	qtc_packaging/debian_harmattan/rules \
	qtc_packaging/debian_harmattan/README \
	qtc_packaging/debian_harmattan/copyright \
	qtc_packaging/debian_harmattan/control \
	qtc_packaging/debian_harmattan/compat \
	qtc_packaging/debian_harmattan/changelog \
	qtc_packaging/debian_harmattan/manifest.aegis \
	qtc_packaging/debian_fremantle/rules \
	qtc_packaging/debian_fremantle/README \
	qtc_packaging/debian_fremantle/copyright \
	qtc_packaging/debian_fremantle/control \
	qtc_packaging/debian_fremantle/compat \
	qtc_packaging/debian_fremantle/changelog
