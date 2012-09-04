#include <QtCore/QDebug>
#include <dbus/dbus-glib-lowlevel.h>

#include "qmafwwatchletvolumecontrol.h"

#define DEFAULT_ADDRESS "unix:path=/var/run/pulse/dbus-socket"

#define VOLUME_SV    "com.Nokia.MainVolume1"
#define VOLUME_PATH  "/com/meego/mainvolume1"
#define VOLUME_IF    "com.Nokia.MainVolume1"

using namespace sowatch;

QMafwWatchletVolumeControl::QMafwWatchletVolumeControl(QObject *parent) :
    QObject(parent), _conn(0), _curStep(0), _maxStep(1)
{
	_openConnection();
}

QMafwWatchletVolumeControl::~QMafwWatchletVolumeControl()
{
	if (_conn) {
		dbus_connection_remove_filter(_conn, handleDBusSignal, this);
		dbus_connection_unref(_conn);
	}
}

int QMafwWatchletVolumeControl::volume() const
{
	return _curStep;
}

int QMafwWatchletVolumeControl::max() const
{
	return _maxStep - 1;
}

int QMafwWatchletVolumeControl::min() const
{
	return 0;
}

void QMafwWatchletVolumeControl::setVolume(int vol)
{
	if (!_conn) return;
	DBusMessage *msg;
	quint32 value = vol;
	const char *volume_if = VOLUME_IF;
	const char *prop = "CurrentStep";

	msg = dbus_message_new_method_call(VOLUME_SV, VOLUME_PATH,
	                                   "org.freedesktop.DBus.Properties", "Set");
	Q_ASSERT(msg);

	dbus_message_append_args(msg,
	                         DBUS_TYPE_STRING, &volume_if,
	                         DBUS_TYPE_STRING, &prop,
	                         DBUS_TYPE_INVALID);

	DBusMessageIter iter, iter_variant;
	dbus_message_iter_init_append(msg, &iter);
	dbus_message_iter_open_container(&iter,
	                                 DBUS_TYPE_VARIANT,
	                                 DBUS_TYPE_UINT32_AS_STRING,
	                                 &iter_variant);
	dbus_message_iter_append_basic(&iter_variant, DBUS_TYPE_UINT32, &value);
	dbus_message_iter_close_container(&iter, &iter_variant);

	dbus_connection_send(_conn, msg, 0);

	dbus_message_unref(msg);
}

void QMafwWatchletVolumeControl::up()
{
	if (_curStep < _maxStep - 1) {
		setVolume(_curStep + 1);
	}
}

void QMafwWatchletVolumeControl::down()
{
	if (_curStep > 0) {
		setVolume(_curStep - 1);
	}
}

void QMafwWatchletVolumeControl::_openConnection()
{
	DBusError err;

	// Allow for an alternative PulseAudio D-Bus server address
	char *pa_bus_address = getenv("PULSE_DBUS_SERVER");
	if (!pa_bus_address)
		pa_bus_address = (char *) DEFAULT_ADDRESS;

    dbus_error_init(&err);

	_conn = dbus_connection_open(pa_bus_address, &err);
	if (!_conn) {
		qWarning() << "Failed to open connection to PulseAudio D-Bus server:"
		           << err.message;
		return;
	}

	dbus_connection_setup_with_g_main(_conn, NULL);
	dbus_connection_add_filter(_conn, handleDBusSignal, this, 0);

	_listenForSignal();
	_fetchValues();
}

void QMafwWatchletVolumeControl::_listenForSignal()
{
	DBusMessage *msg;
	const char *signal = "com.Nokia.MainVolume1.StepsUpdated";
	char **empty_array = { 0 };

	msg = dbus_message_new_method_call(NULL, "/org/pulseaudio/core1", NULL,
	                                   "ListenForSignal");

	if (!msg) {
		qWarning() << "Cannot create ListenForSignal message";
		return;
	}

	dbus_message_append_args(msg,
	                         DBUS_TYPE_STRING, &signal,
	                         DBUS_TYPE_ARRAY, DBUS_TYPE_OBJECT_PATH, &empty_array, 0,
	                         DBUS_TYPE_INVALID);
	dbus_connection_send(_conn, msg, 0);

	dbus_message_unref(msg);
}

void QMafwWatchletVolumeControl::_fetchValues()
{
	DBusMessage *msg, *reply;
	DBusError err;

	dbus_error_init(&err);

	msg = dbus_message_new_method_call(VOLUME_SV, VOLUME_PATH,
	                                   "org.freedesktop.DBus.Properties",
	                                   "GetAll");
	if (!msg) {
		qWarning() << "Cannot create GetAll message";
		return;
	}


	const char *volume_if = VOLUME_IF;
	dbus_message_append_args(msg,
	                         DBUS_TYPE_STRING, &volume_if,
	                         DBUS_TYPE_INVALID);

	DBusPendingCall *pending;
	if (!dbus_connection_send_with_reply(_conn, msg, &pending, -1)) {
		qWarning() << "Cannot send GetAll message";
		return;
	}

	dbus_message_unref(msg);

	if (pending) {
		if (!dbus_pending_call_set_notify(pending, handleFetchReply, this, 0)) {
			qWarning() << "Cannot set GetAll notify";
			return;
		}
	}
}

void QMafwWatchletVolumeControl::handleFetchReply(DBusPendingCall *pending, void *user_data)
{
	QMafwWatchletVolumeControl *self = static_cast<QMafwWatchletVolumeControl*>(user_data);
	DBusMessage *reply = dbus_pending_call_steal_reply(pending);

	if (!reply || dbus_message_get_type(reply) != DBUS_MESSAGE_TYPE_METHOD_RETURN) {
		qWarning() << "Received something that is not a method return";
	}

	DBusMessageIter iter;
	dbus_message_iter_init(reply, &iter);
	while (dbus_message_iter_get_arg_type(&iter) != DBUS_TYPE_INVALID) {
		DBusMessageIter iter_dict;
		Q_ASSERT(dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_ARRAY);
		dbus_message_iter_recurse(&iter, &iter_dict);

		while (dbus_message_iter_get_arg_type(&iter_dict) != DBUS_TYPE_INVALID) {
			DBusMessageIter iter_entry;
			Q_ASSERT(dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_ARRAY);
			dbus_message_iter_recurse(&iter_dict, &iter_entry);

			char *prop_name;
			Q_ASSERT(dbus_message_iter_get_arg_type(&iter_entry) == DBUS_TYPE_STRING);
			dbus_message_iter_get_basic(&iter_entry, &prop_name);
			dbus_message_iter_next(&iter_entry);

			Q_ASSERT(dbus_message_iter_get_arg_type(&iter_entry) == DBUS_TYPE_VARIANT);
			DBusMessageIter iter_variant;
			dbus_message_iter_recurse(&iter_entry, &iter_variant);

			if (prop_name &&
			        dbus_message_iter_get_arg_type(&iter_variant) == DBUS_TYPE_UINT32) {
				quint32 value;
				dbus_message_iter_get_basic(&iter_variant, &value);

				if (strcmp(prop_name, "StepCount")) {
					self->_maxStep = value;
				} else if (strcmp(prop_name, "CurrentStep")) {
					self->_curStep = value;
				}
				qDebug() << prop_name << value;
			}

			dbus_message_iter_next(&iter_dict);
		}

		dbus_message_iter_next(&iter);
	}

	emit self->maxChanged();
	emit self->volumeChanged();

	dbus_message_unref(reply);
	dbus_pending_call_unref(pending);
}

DBusHandlerResult QMafwWatchletVolumeControl::handleDBusSignal(DBusConnection *connection, DBusMessage *message, void *user_data)
{
	QMafwWatchletVolumeControl *self = static_cast<QMafwWatchletVolumeControl*>(user_data);
	if (dbus_message_is_signal(message, VOLUME_IF, "StepsUpdated")) {
		DBusError err;
		quint32 curStep, maxStep;

		dbus_error_init(&err);
		if (dbus_message_get_args(message, &err,
		                          DBUS_TYPE_UINT32, &maxStep,
		                          DBUS_TYPE_UINT32, &curStep,
		                          DBUS_TYPE_INVALID)) {
			if (self->_maxStep != maxStep) {
				self->_maxStep = maxStep;
				emit self->maxChanged();
			}
			if (self->_curStep != curStep) {
				self->_curStep = curStep;
				emit self->volumeChanged();
			}
		}
	}
}
