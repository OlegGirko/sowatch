import QtQuick 1.0
import com.javispedro.sowatch 1.0
import com.javispedro.sowatch.metawatch 1.0

MWPage {
	id: page

	Column {
		Item {
			id: systemArea
			// Firmware draws top 30 lines;
			// ensure we do not draw anything on top
			width: page.width
			height: 30
		}

		Image {
			width: page.width
			height: 2
			source: "idle_border.png"
		}

		Item {
			width: page.width
			height: 30
			// TODO Weather stuff.
		}

		Image {
			width: page.width
			height: 2
			source: "idle_border.png"
		}

		Item {
			width: page.width
			height: 2
		}

		Row {
			anchors.horizontalCenter: parent.horizontalCenter
			height: 30
			spacing: 8
			Column {
				spacing: 4
				Image {
					width: 24
					height: 18
					source: "idle_call.png"
				}
				Text {
					id: labelCalls
					anchors.horizontalCenter: parent.horizontalCenter
					font.family: "MetaWatch Large caps 8pt"
					font.pixelSize: 8
					text: "0"
				}
			}
			Column {
				spacing: 2
				Image {
					width: 24
					height: 18
					source: "idle_msg.png"
				}
				Text {
					id: labelMsgs
					anchors.horizontalCenter: parent.horizontalCenter
					font.family: "MetaWatch Large caps 8pt"
					font.pixelSize: 8
					text: "0"
				}
			}
			Column {
				spacing: 2
				Image {
					width: 24
					height: 18
					source: "idle_mail.png"
				}
				Text {
					id: labelMails
					anchors.horizontalCenter: parent.horizontalCenter
					font.family: "MetaWatch Large caps 8pt"
					font.pixelSize: 8
					text: "0"
				}
			}
		}
	}

	function updateUnreadCounts() {
		labelCalls.text = notifications.fullCountByType(Notification.MissedCallNotification);
		labelMsgs.text = notifications.fullCountByType(Notification.SmsNotification) +
				notifications.fullCountByType(Notification.MmsNotification) +
				notifications.fullCountByType(Notification.ImNotification);
		labelMails.text = notifications.fullCountByType(Notification.EmailNotification);
		console.log("unread mails = " + labelMails.text);
	}

	function updateWeather() {
		var weather = notifications.getMostRecentByType(Notification.WeatherNotification);
		if (typeof weather !== "undefined") {
			// TODO Weather stuff
		}
	}

	function update() {
		updateUnreadCounts();
		updateWeather();
	}

	Connections {
		target: notifications
		onModelChanged: update();
	}
}
