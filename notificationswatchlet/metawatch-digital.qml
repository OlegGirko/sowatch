import QtQuick 1.0
import QtMobility.location 1.2

Rectangle {
	width: 96
	height: 96

	color: "white"

	ListView {
		id: notifs
		anchors.fill: parent
		model: watch.notifications
		delegate: Column {
			Text { text: model.modelData.title }
			Text { text: model.modelData.body }
		}
		visible: count > 0;
	}
	Text {
		anchors.fill: parent
		text: "No notifications"
		visible: notifs.count == 0;
	}

	Connections {
		target: watch
		onButtonPressed : {
			switch(button) {
			case 1:
				notifs.decrementCurrentIndex();
				break;
			case 2:
				notifs.incrementCurrentIndex();
				break;
			}
		}
	}
}
