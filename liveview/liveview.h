#ifndef LIVEVIEW_H
#define LIVEVIEW_H

#include <sowatch.h>
#include <sowatchbt.h>

namespace sowatch
{

class LiveViewPaintEngine;

class LiveView : public BluetoothWatch
{
	Q_OBJECT

public:
	explicit LiveView(ConfigKey *settings, QObject *parent = 0);
	~LiveView();

	QPaintEngine* paintEngine() const;
	int metric(PaintDeviceMetric metric) const;

	QString model() const;
	QStringList buttons() const;

	bool busy() const;

	void setDateTime(const QDateTime& dateTime);
	void queryDateTime();
	QDateTime dateTime() const;

	void queryBatteryLevel();
	int batteryLevel() const;

	void queryCharging();
	bool charging() const;

	void displayIdleScreen();
	void displayNotification(Notification *notification);
	void displayApplication();

	void vibrate(int msecs);

	void setWatchletsModel(WatchletsModel *model);
	void setNotificationsModel(NotificationsModel *model);

	static const int MaxBitmapSize;

	// Only for application mode
	QImage* image();
	/** Render a image in a certain position. */
	void renderImage(int x, int y, const QImage& image);
	/** Clear the current display to black. */
	void clear();

protected:
	static const int DelayBetweenMessages = 200;

	enum MessageType {
		NoMessage = 0,
		GetDisplayProperties = 1,
		GetDisplayPropertiesResponse = 2,
		DeviceStatusChange = 7,
		DeviceStatusChangeResponse = 8,
		DisplayBitmap = 19,
		DisplayBitmapResponse = 20,
		DisplayClear = 21,
		DisplayClearResponse = 22,
		SetMenuSize = 23,
		SetMenuSizeResponse = 24,
		MenuItemRequest = 25,
		MenuItemResponse = 26,
		NotificationRequest = 27,
		NotificationResponse = 28,
		Navigation = 29,
		NavigationResponse = 30,
		MenuItemsRequest = 35,
		DateTimeRequest = 38,
		DateTimeResponse = 39,
		EnableLed = 40,
		EnableLedResponse = 41,
		Ack = 44,
		GetSoftwareVersion = 68,
		GetSoftwareVersionResponse = 69
	};

	enum ResponseType {
		ResponseOk = 0,
		ResponseError = 1,
		ResponseOutOfMemory = 2,
		ResponseExit = 3,
		ResponseCancel = 4
	};

	enum DeviceStatus {
		DeviceOff = 0,
		DeviceOn = 1,
		DeviceMenu = 2
	};

	enum MenuItemType {
		MenuNotificationList = 0,
		MenuOther = 1
	};

	enum Mode {
		RootMenuMode = 0,
		ApplicationMode,
		NotificationListMode
	};

	enum NavigationEvent {
		UpPress = 1,
		DownPress = 4,
		LeftPress = 7,
		RightPress = 10,
		SelectPress = 13,
		SelectLongPress = 14,
		SelectDoublePress = 15,
		SelectMenu = 32
	};

	enum NotificationAction {
		NotificationShowCurrent = 0,
		NotificationShowFirst = 1,
		NotificationShowLast = 2,
		NotificationShowNext = 3,
		NotificationShowPrev = 4
	};

	struct Message {
		MessageType type;
		QByteArray data;
		Message(MessageType ntype = NoMessage, QByteArray ndata = QByteArray()) :
			type(ntype), data(ndata)
		{ }
	};

	struct RootMenuNotificationItem {
		QByteArray icon;
		QString title;
		QList<Notification::Type> notificationTypes;
	};

	struct RootMenuItem {
		MenuItemType type;
		QByteArray icon;
		QString title;
		int unread;
		QString watchletId;
		const RootMenuNotificationItem *notificationItem;
	};

	static QMap<MessageType, MessageType> _ackMap;
	static void initializeAckMap();
	static MessageType ackForMessage(MessageType type);

	static QList<RootMenuNotificationItem> _rootNotificationItems;
	static void initializeRootNotificationItems();

	void setupBluetoothWatch();
	void desetupBluetoothWatch();


	void recreateNotificationsMenu();
	/** Recreate the device menu (after watchlets change) */
	void recreateWatchletsMenu();
	/** Update the device menu (after a power on, etc.) */
	void refreshMenu();

	static QByteArray encodeImage(const QImage& image);
	static QByteArray encodeImage(const QUrl& url);

protected:
	void send(const Message& msg);
	void sendResponse(MessageType type, ResponseType response);

	void updateDisplayProperties();
	void updateSoftwareVersion();
	void displayBitmap(unsigned char x, unsigned char y, const QByteArray& image);
	void displayClear();
	void setMenuSize(unsigned char size);
	void sendMenuItem(unsigned char id, MenuItemType type, unsigned short unread, const QString& text, const QByteArray& image);
	void sendNotification(unsigned short id, unsigned short unread, unsigned short count, const QString& date, const QString& header, const QString& body, const QByteArray& image);
	void enableLed(const QColor& color, unsigned short delay, unsigned short time);

	void handleMessage(const Message& msg);
	void handleDeviceStatusChange(const Message& msg);
	void handleMenuItemRequest(const Message& msg);
	void handleNotificationRequest(const Message& msg);
	void handleNavigation(const Message& msg);
	void handleMenuItemsRequest(const Message& msg);
	void handleDateTimeRequest(const Message& msg);
	void handleDisplayProperties(const Message& msg);
	void handleSoftwareVersion(const Message& msg);

private:
	void sendMessageFromQueue();

private slots:
	void handleDataReceived();
	void handleWatchletsChanged();
	void handleNotificationsChanged();

private:
	ConfigKey *_settings;
	WatchletsModel *_watchlets;
	NotificationsModel *_notifications;

	bool _24hMode : 1;

	int _screenWidth;
	int _screenHeight;
	QStringList _buttons;

	Mode _mode;
	int _curNotificationIndex;

	// Required by QPaintDevice
	mutable LiveViewPaintEngine* _paintEngine;
	QImage _image;

	QList<RootMenuItem> _rootMenu;
	/** Keeps the index of the first watchlet. */
	int _rootMenuFirstWatchlet;

	/** Outgoing message queue. */
	QQueue<Message> _sendingMsgs;
	MessageType _waitingForAck;
	/** Incomplete message that is being received. */
	Message _receivingMsg;
};

}

#endif // LIVEVIEW_H
