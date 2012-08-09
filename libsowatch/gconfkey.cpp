#include <QtCore/QDebug>

#include <gconf/gconf-client.h>
#include <gconf/gconf-value.h>

#include "gconfkey.h"

using namespace sowatch;

static GConfClient* g_client = NULL;

static GConfClient* get_client() {
	if (!g_client) {
		g_type_init();
		g_client = gconf_client_get_default();
	}
	return g_client;
}

static QVariant convert_value(GConfValue *gval)
{
	switch (gval->type) {
	case GCONF_VALUE_STRING:
		return QVariant(QString::fromUtf8(gconf_value_get_string(gval)));
	case GCONF_VALUE_INT:
		return QVariant(gconf_value_get_int(gval));
	case GCONF_VALUE_FLOAT:
		return QVariant(gconf_value_get_float(gval));
	case GCONF_VALUE_BOOL:
		return QVariant(gconf_value_get_bool(gval) ? true : false);
	case GCONF_VALUE_LIST:
		if (gconf_value_get_list_type(gval) == GCONF_VALUE_STRING) {
			QStringList r;
			for (GSList *l = gconf_value_get_list(gval); l; l = l->next) {
				GConfValue* lgval = (GConfValue *) l->data;
				r.append(QString::fromUtf8(gconf_value_get_string(lgval)));
			}
			return QVariant::fromValue(r);
		} else {
			QVariantList r;
			for (GSList *l = gconf_value_get_list(gval); l; l = l->next) {
				GConfValue* lgval = (GConfValue *) l->data;
				r.append(convert_value(lgval));
			}
			return QVariant::fromValue(r);
		}
	default:
		return QVariant();
	}
}

static GConfValue * convert_value(const QVariant& v)
{
	GConfValue *gval;
	switch (v.type()) {
	case QVariant::String:
		gval = gconf_value_new(GCONF_VALUE_STRING);
		gconf_value_set_string(gval, v.toString().toUtf8().constData());
		break;
	case QVariant::Int:
		gval = gconf_value_new(GCONF_VALUE_INT);
		gconf_value_set_int(gval, v.toInt());
		break;
	case QVariant::Double:
		gval = gconf_value_new(GCONF_VALUE_FLOAT);
		gconf_value_set_float(gval, v.toDouble());
		break;
	case QVariant::Bool:
		gval = gconf_value_new(GCONF_VALUE_BOOL);
		gconf_value_set_bool(gval, v.toBool());
		break;
	case QVariant::StringList: {
		QStringList sl = v.toStringList();
		GSList *glist = NULL;
		gval = gconf_value_new(GCONF_VALUE_LIST);
		gconf_value_set_list_type(gval, GCONF_VALUE_STRING);
		foreach (const QString& s, sl) {
			GConfValue *lgval = gconf_value_new(GCONF_VALUE_STRING);
			gconf_value_set_string(lgval, s.toUtf8().constData());
			glist = g_slist_prepend(glist, lgval);
		}
		gconf_value_set_list_nocopy(gval, g_slist_reverse(glist));
		}
		break;
	// TODO: QVariantList, anything else.
	default:
		gval = NULL;
		break;
	}

	return gval;
}

static QByteArray convert_key(const QString &key)
{
	return key.toAscii();
}

static QString convert_key(const gchar *key)
{
	return QString::fromAscii(key);
}

static void notify_func(GConfClient* client, guint cnxn_id, GConfEntry *entry, gpointer user_data)
{
	Q_UNUSED(client);
	Q_UNUSED(cnxn_id);
	GConfKey* key = static_cast<GConfKey*>(user_data);
	key->notifyChanged(convert_key(entry->key));
}

static QString get_basename(const QString& path)
{
	int pos = path.lastIndexOf('/');
	if (pos >= 0) {
		return path.mid(pos+1);
	} else {
		return path;
	}
}

GConfKey::GConfKey(const QString& key, QObject *parent) :
    ConfigKey(parent), _key(key), _notify(0)
{
	if (_key.endsWith("/")) {
		_key.chop(1);
	}
}

GConfKey::~GConfKey()
{
	if (_notify) {
		GConfClient* client = get_client();
		gconf_client_remove_dir(client, getNativeKey(), NULL);
		gconf_client_notify_remove(client, _notify);
	}
}

QString GConfKey::key() const
{
	return _key;
}

void GConfKey::setKey(const QString &key)
{
	_key = key;
	if (_key.endsWith("/")) {
		_key.chop(1);
	}
	emit keyChanged();
	emit changed();
}

QVariant GConfKey::value() const
{
	return value(QString());
}

void GConfKey::set(const QVariant &value)
{
	set(QString(), value);
}

void GConfKey::unset()
{
	unset(QString());
}

bool GConfKey::isSet() const
{
	return isSet(QString());
}

bool GConfKey::isDir() const
{
	return isDir(QString());
}

QVariant GConfKey::value(const QString &subkey) const
{
	const QString path = fullpath(subkey);
	GConfValue *gval = gconf_client_get(get_client(), convert_key(path), NULL);
	if (!gval) {
		return QVariant();
	}
	QVariant v = convert_value(gval);
	gconf_value_free(gval);
	return v;
}

QVariant GConfKey::value(const QString &subkey, const QVariant &def) const
{
	const QString path = fullpath(subkey);
	GConfValue *gval = gconf_client_get_without_default(get_client(), convert_key(path), NULL);
	if (!gval) {
		return def;
	}
	QVariant v = convert_value(gval);
	gconf_value_free(gval);
	return v;
}

void GConfKey::set(const QString &subkey, const QVariant &value)
{
	const QString path = fullpath(subkey);
	GConfValue *gval = convert_value(value);
	gconf_client_set(get_client(), convert_key(path), gval, NULL);
	gconf_value_free(gval);
}

void GConfKey::unset(const QString &subkey)
{
	const QString path = fullpath(subkey);
	gconf_client_unset(get_client(), convert_key(path), NULL);
}

bool GConfKey::isSet(const QString &subkey) const
{
	const QString path = fullpath(subkey);
	GConfValue *gval = gconf_client_get_without_default(get_client(), convert_key(path), NULL);
	if (gval) {
		gconf_value_free(gval);
		return true;
	} else {
		return false;
	}
}

bool GConfKey::isDir(const QString &subkey) const
{
	const QString path = fullpath(subkey);
	return gconf_client_dir_exists(get_client(), convert_key(path), NULL);
}

QStringList GConfKey::dirs() const
{
	QStringList r;
	GSList *l = gconf_client_all_dirs(get_client(), getNativeKey(), NULL);
	for (GSList *i = l; i; i = i->next) {
		QString path = QString::fromAscii(static_cast<char*>(i->data));
		r.append(get_basename(path));
		g_free(i->data);
	}
	g_slist_free(l);
	return r;
}

QStringList GConfKey::keys() const
{
	QStringList r;
	GSList *l = gconf_client_all_entries(get_client(), getNativeKey(), NULL);
	for (GSList *i = l; i; i = i->next) {
		GConfEntry *e = static_cast<GConfEntry*>(i->data);
		QString path = QString::fromAscii(e->key);
		r.append(get_basename(path));
		gconf_entry_free(e);
	}
	g_slist_free(l);
	return r;
}

void GConfKey::recursiveUnset()
{
	gconf_client_recursive_unset(get_client(), getNativeKey(),
	                             static_cast<GConfUnsetFlags>(0), NULL);
}

ConfigKey* GConfKey::getSubkey(const QString &subkey, QObject *parent) const
{
	return new GConfKey(fullpath(subkey), parent);
}

void GConfKey::notifyChanged(const QString& key)
{
	if (key == _key) {
		emit changed();
	} else if (key.startsWith(_key + '/')) {
		emit subkeyChanged(key.mid(_key.size() + 1));
	}
}

void GConfKey::connectNotify(const char *signal)
{
	Q_UNUSED(signal);
	if (!_notify) {
		GConfClient* client = get_client();
		gconf_client_add_dir(client, getNativeKey(), GCONF_CLIENT_PRELOAD_NONE, NULL);
		_notify = gconf_client_notify_add(client, getNativeKey(), notify_func,
		                                  this, NULL, NULL);
	}
}

QString GConfKey::fullpath(const QString &child) const
{
	if (child.isEmpty()) {
		return _key;
	} else {
		return _key + '/' + child;
	}
}

QByteArray GConfKey::getNativeKey() const
{
	return convert_key(_key);
}
