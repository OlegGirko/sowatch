#include <dconf.h>

#include <QtCore/QDebug>

#include "dconfkey.h"

using namespace sowatch;

static DConfClient* g_client = NULL;

static DConfClient* get_client() {
	if (!g_client)
		g_client = dconf_client_new();
	return g_client;
}

static QVariant convert_value(GVariant *value)
{
	switch (g_variant_classify(value)) {
	case G_VARIANT_CLASS_BOOLEAN:
		return QVariant::fromValue(bool(g_variant_get_boolean(value)));
	case G_VARIANT_CLASS_BYTE:
		return QVariant::fromValue(char(g_variant_get_byte(value)));
	case G_VARIANT_CLASS_INT16:
		return QVariant::fromValue(g_variant_get_int16(value));
	case G_VARIANT_CLASS_UINT16:
		return QVariant::fromValue(g_variant_get_uint16(value));
	case G_VARIANT_CLASS_INT32:
		return QVariant::fromValue(g_variant_get_int32(value));
	case G_VARIANT_CLASS_UINT32:
		return QVariant::fromValue(g_variant_get_uint32(value));
	case G_VARIANT_CLASS_INT64:
		return QVariant::fromValue(g_variant_get_int64(value));
	case G_VARIANT_CLASS_UINT64:
		return QVariant::fromValue(g_variant_get_uint64(value));
	case G_VARIANT_CLASS_DOUBLE:
		return QVariant::fromValue(g_variant_get_double(value));
	case G_VARIANT_CLASS_STRING:
		return QVariant::fromValue(QString::fromUtf8(g_variant_get_string(value, 0)));
	case G_VARIANT_CLASS_ARRAY: {
		const GVariantType *type = g_variant_get_type(value);
		if (g_variant_type_equal(type, G_VARIANT_TYPE_BYTESTRING)) {
			return QVariant::fromValue(QByteArray(g_variant_get_bytestring(value)));
		} else if (g_variant_type_equal(type, G_VARIANT_TYPE_STRING_ARRAY)) {
			QStringList stringList;
			gsize length = 0;
			const gchar **strings = g_variant_get_strv(value, &length);
			for (gsize i = 0; i < length ; ++i)
				stringList.append(QString::fromUtf8(strings[i]));
			g_free(strings);
			return stringList;
		} else if (g_variant_type_equal(type, G_VARIANT_TYPE_BYTESTRING_ARRAY)) {
			QList<QByteArray> stringList;
			gsize length = 0;
			const gchar **strings = g_variant_get_bytestring_array (value, &length);
			for (gsize i = 0; i < length ; ++i)
				stringList.append(strings[i]);
			g_free(strings);
			return QVariant::fromValue(stringList);
		} else if (g_variant_type_equal(type, G_VARIANT_TYPE_VARDICT)) {
			QVariantMap variantMap;
			for (gsize i = 0, count = g_variant_n_children(value); i < count; ++i) {
				GVariant *child = g_variant_get_child_value(value, i);
				GVariant *k = g_variant_get_child_value(child, 0);
				GVariant *v = g_variant_get_child_value(child, 1);

				variantMap.insert(QString::fromUtf8(g_variant_get_string(k, 0)), convert_value(v));
				g_variant_unref(k);
				g_variant_unref(v);
				g_variant_unref(child);
			}
			return variantMap;
		} else {
			QVariantList variantList;
			for (gsize i = 0, count = g_variant_n_children(value); i < count; ++i) {
				GVariant *child = g_variant_get_child_value(value, i);
				variantList.append(convert_value(child));
				g_variant_unref(child);
			}
			return variantList;
		}
	}
	default:
		return QVariant();
	}
}

static GVariant * convert_value(const QVariant& v)
{
	switch (v.type()) {
	case QMetaType::QString:
		return g_variant_new_string(v.toString().toUtf8().constData());
	case QMetaType::Int:
		return g_variant_new_int32(v.value<qint32>());
	case QMetaType::Float:
	case QMetaType::Double:
		return g_variant_new_double(v.value<double>());
	case QMetaType::Bool:
		return g_variant_new_boolean(v.toBool());
	case QVariant::StringList: {
		QList<QByteArray> utf8List;
		QVector<const char *> pointers;
		foreach (const QString &string, v.toStringList()) {
			const QByteArray utf8 = string.toUtf8();
			utf8List.append(utf8);
			pointers.append(utf8.constData());
		}
		return g_variant_new_strv(pointers.data(), pointers.count());
	}
	// TODO: QVariantList, anything else.
	default:
		return NULL;
	}
}

static QByteArray convert_key(const QString &key)
{
	return key.toLocal8Bit();
}

static QString convert_key(const gchar *key)
{
	return QString::fromLocal8Bit(key);
}

static void notify_func(DConfClient* client, gchar *prefix, GStrv changes, gchar *tag, gpointer user_data)
{
	Q_UNUSED(client);
	Q_UNUSED(changes);
	Q_UNUSED(tag);
	DConfKey* key = static_cast<DConfKey*>(user_data);
	key->notifyChanged(convert_key(prefix));
}

DConfKey::DConfKey(const QString& key, QObject *parent) :
    ConfigKey(parent), _key(key), _notify(0)
{
	if (_key.endsWith("/")) {
		_key.chop(1);
	}
}

DConfKey::~DConfKey()
{
	if (_notify) {
		DConfClient* client = get_client();
		dconf_client_unwatch_fast(client, getNativeKey());
	}
}

QString DConfKey::key() const
{
	return _key;
}

void DConfKey::setKey(const QString &key)
{
	_key = key;
	if (_key.endsWith("/")) {
		_key.chop(1);
	}
	emit keyChanged();
	emit changed();
}

QVariant DConfKey::value() const
{
	return value(QString());
}

void DConfKey::set(const QVariant &value)
{
	set(QString(), value);
}

void DConfKey::unset()
{
	unset(QString());
}

bool DConfKey::isSet() const
{
	return isSet(QString());
}

bool DConfKey::isDir() const
{
	return isDir(QString());
}

QVariant DConfKey::value(const QString &subkey) const
{
	const QString path = fullpath(subkey);
	GVariant *gval = dconf_client_read(get_client(), convert_key(path));
	if (!gval) {
		return QVariant();
	}
	QVariant v = convert_value(gval);
	g_variant_unref(gval);
	return v;
}

QVariant DConfKey::value(const QString &subkey, const QVariant &def) const
{
	const QString path = fullpath(subkey);
	GVariant *gval = dconf_client_read(get_client(), convert_key(path));
	if (!gval) {
		return def;
	}
	QVariant v = convert_value(gval);
	g_variant_unref(gval);
	return v;
}

void DConfKey::set(const QString &subkey, const QVariant &value)
{
	GError *error = 0;
	const QString path = fullpath(subkey);
	GVariant *gval = convert_value(value);
	if (!gval) {
		qWarning() << "DConf: no conversion for " << subkey << value;
		return;
	}
	dconf_client_write_fast(get_client(), convert_key(path), gval, &error);
	if (error) {
		qWarning() << "DConf: Failed to write value for " << subkey << value;
		qWarning() << error->message;
		g_error_free(error);
	}
	g_variant_unref(gval);
}

void DConfKey::unset(const QString &subkey)
{
	const QString path = fullpath(subkey);
	dconf_client_write_fast(get_client(), convert_key(path), 0, 0);
}

bool DConfKey::isSet(const QString &subkey) const
{
	const QString path = fullpath(subkey);
	GVariant *gval = dconf_client_read(get_client(), convert_key(path));
	if (gval) {
		g_variant_unref(gval);
		return true;
	} else {
		return false;
	}
}

bool DConfKey::isDir(const QString &subkey) const
{
	const QString path = fullpath(subkey);
	gchar **list = dconf_client_list(get_client(), convert_key(path), 0);
	bool result = list[0];
	g_strfreev(list);
	return result;
}

QStringList DConfKey::dirs() const
{
	QStringList r;
	gchar **l = dconf_client_list(get_client(), getNativeKey(), NULL);
	for (gchar **p = l; *p != NULL; p++)
		if ((*p)[0] && (*p)[strlen(*p)-1] == '/')
			r.append(convert_key(*p));
	g_strfreev(l);
	return r;
}

QStringList DConfKey::keys() const
{
	QStringList r;
	gchar **l = dconf_client_list(get_client(), getNativeKey(), NULL);
	for (gchar **p = l; *p != NULL; p++)
		if (!(*p)[0] || (*p)[strlen(*p)-1] != '/')
			r.append(convert_key(*p));
	g_strfreev(l);
	return r;
}

void DConfKey::recursiveUnset()
{
	dconf_client_write_fast(get_client(), getNativeKey(), 0, 0);
}

ConfigKey* DConfKey::getSubkey(const QString &subkey, QObject *parent) const
{
	return new DConfKey(fullpath(subkey), parent);
}

void DConfKey::notifyChanged(const QString& key)
{
	if (key == _key) {
		emit changed();
	} else if (key.startsWith(_key + '/')) {
		emit subkeyChanged(key.mid(_key.size() + 1));
	}
}

void DConfKey::connectNotify(const char *signal)
{
	Q_UNUSED(signal);
	if (!_notify) {
		DConfClient* client = get_client();
		dconf_client_watch_fast(client, getNativeKey());
		g_signal_connect(client, "changed", G_CALLBACK(notify_func), this);
	}
}

QString DConfKey::fullpath(const QString &child) const
{
	if (child.isEmpty()) {
		return _key;
	} else {
		return _key + '/' + child;
	}
}

QByteArray DConfKey::getNativeKey() const
{
	return convert_key(_key);
}
