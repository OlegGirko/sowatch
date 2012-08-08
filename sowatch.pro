TEMPLATE = subdirs
SUBDIRS = libsowatch

# The MetaWatch driver plugin
SUBDIRS += metawatch
metawatch.depends = libsowatch

# Some watchlets
SUBDIRS += notificationswatchlet sysinfowatchlet
#SUBDIRS += qmsgwatchlet qmapwatchlet
notificationswatchlet.depends = libsowatch
sysinfowatchlet.depends = libsowatch
qmsgwatchlet.depends = libsowatch
qmapwatchlet.depends = libsowatch

unix {
	SUBDIRS += sowatchd
	SUBDIRS += sowatchui

	sowatchd.depends = libsowatch
	sowatchui.depends = libsowatch sowatchd
}

contains(MEEGO_EDITION,harmattan) {
	SUBDIRS += meegohandsetnotification ckitcallnotification harmaccuweather
	#SUBDIRS += qmafwwatchlet

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
