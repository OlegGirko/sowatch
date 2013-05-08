TEMPLATE = subdirs

# Please comment subdirectories you do not want to build as desired

# Core library
# This is mandatory. Depends on Qt and GConf.
SUBDIRS = libsowatch

SUBDIRS += libsowatchbt
libsowatchbt.depends = libsowatch

# The MetaWatch driver plugin
# Depends on Qt Mobility SystemInfo and Bluetooth.
SUBDIRS += metawatch metawatchwatchlets
metawatch.depends = libsowatch libsowatchbt
metawatchwatchlets.depends = metawatch

# LiveView driver plugin
SUBDIRS += liveview
liveview.depends = libsowatch libsowatchbt

# Some watchlets
# This just shows a list of pending notifications and has no dependencies.
SUBDIRS += notificationswatchlet
notificationswatchlet.depends = libsowatch

# This shows some values from Qt SystemInfo on the watch
SUBDIRS += sysinfowatchlet
sysinfowatchlet.depends = libsowatch

# This shows some inbox messages using QtMobility
SUBDIRS += qmsgwatchlet
qmsgwatchlet.depends = libsowatch

# This shows some calendar appointments using QtMobility
SUBDIRS += qorgwatchlet
qorgwatchlet.depends = libsowatch

# This shows a map around the current position using QtMobility Mapping features
SUBDIRS += qmapwatchlet
qmapwatchlet.depends = libsowatch

# Toy watchlets
# Shows a cat running around. No dependencies.
SUBDIRS += nekowatchlet
nekowatchlet.depends = libsowatch

unix {
	# These use D-Bus for interprocess communication.
	# The main daemon.
	SUBDIRS += sowatchd
	# Configuration interface.
	SUBDIRS += sowatchui

	sowatchd.depends = libsowatch
	sowatchui.depends = libsowatch sowatchd
}

# Harmattan specific stuff
contains(MEEGO_EDITION,harmattan) {
	# Obtain notifications from the Meego Handset spec
	SUBDIRS += meegohandsetnotification
	# Obtain notifications of in-progress calls
	SUBDIRS += ckitcallnotification
	# Obtain weather information from the Harmattan weather app
	SUBDIRS += harmaccuweather
	# Obtain weather information from MeeCast
	SUBDIRS += meecastweather
	# Control the Harmattan music player
	SUBDIRS += qmafwwatchlet

	meegohandsetnotification.depends = libsowatch
	ckitcallnotification.depends = libsowatch
	harmaccuweather.depends = libsowatch
	meecastweather.depends = libsowatch
	qmafwwatchlet.depends = libsowatch
}

# Debug only watchlets
CONFIG(debug, debug|release) {
	SUBDIRS += testnotification
	testnotification.depends = libsowatch
}

# Packaging stuff
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
