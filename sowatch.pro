TEMPLATE = subdirs

# Core library
SUBDIRS = libsowatch

# The MetaWatch driver plugin
SUBDIRS += metawatch
metawatch.depends = libsowatch

# Some watchlets
SUBDIRS += notificationswatchlet sysinfowatchlet
SUBDIRS += qmsgwatchlet
SUBDIRS += qmapwatchlet
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
	# Harmattan specific stuff
	SUBDIRS += meegohandsetnotification ckitcallnotification harmaccuweather
	SUBDIRS += qmafwwatchlet

	meegohandsetnotification.depends = libsowatch
	ckitcallnotification.depends = libsowatch
	harmaccuweather.depends = libsowatch
    qmafwwatchlet.depends = libsowatch
} else:simulator {
	# This notification provider builds almost everywhere so it's good enough as testcase
	SUBDIRS += harmaccuweather
	harmaccuweather.depends = libsowatch
}

OTHER_FILES += \
	qtc_packaging/debian_harmattan/rules \
	qtc_packaging/debian_harmattan/README \
	qtc_packaging/debian_harmattan/copyright \
	qtc_packaging/debian_harmattan/control \
	qtc_packaging/debian_harmattan/compat \
	qtc_packaging/debian_harmattan/changelog \
	qtc_packaging/debian_harmattan/manifest.aegis \
	qtc_packaging/debian_harmattan/prerm \
	qtc_packaging/debian_harmattan/postinst \
	qtc_packaging/debian_fremantle/rules \
	qtc_packaging/debian_fremantle/README \
	qtc_packaging/debian_fremantle/copyright \
	qtc_packaging/debian_fremantle/control \
	qtc_packaging/debian_fremantle/compat \
	qtc_packaging/debian_fremantle/changelog
