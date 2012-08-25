import QtQuick 1.0
import QtMobility.location 1.2
import com.javispedro.sowatch.metawatch 1.0
import com.javispedro.sowatch.qmap 1.0

MWPage {
	id: page

	function formatSpeed(speed) {
		var kmh = speed * 3.6;
		if (kmh < 0 || isNaN(kmh)) {
			return "";
		} else if (kmh < 10) {
			return kmh.toFixed(1);
		} else {
			return kmh.toFixed(0);
		}
	}

	function formatAltitude(alt) {
		if (isNaN(alt)) {
			return "";
		} else {
			return alt.toFixed(0);
		}
	}

	Column {
		anchors.top: parent.top
		width: parent.width

		spacing: 2

		MWTitle {
			text: qsTr("Compass")
			icon.source: "compass-icon.png"
		}

		CompassView {
			anchors.horizontalCenter: parent.horizontalCenter
			id: compass
			updateEnabled: watch.active
			updateInterval: 3000
			width: 48
			height: 48
		}

		Row {
			Column {
				width: page.width / 2
				id: speedColumn

				MWLabel {
					anchors.horizontalCenter: parent.horizontalCenter
					text: qsTr("Speed")
				}
				Row {
					anchors.horizontalCenter: parent.horizontalCenter
					spacing: 2
					MWLabel {
						id: speedLabel
						anchors.top: parent.top
						text: formatSpeed(compass.currentSpeed)
						font.pointSize: 12
					}
					MWLabel {
						anchors.top: parent.top
						text: altitudeLabel.text ? qsTr("km<br>h") : "";
						font.family: "MetaWatch Small caps 8pt"
						font.pointSize: 6
					}
				}
			}
			Column {
				width: page.width / 2
				id: altitudeColumn

				MWLabel {
					anchors.horizontalCenter: parent.horizontalCenter
					text: qsTr("Altitude")
				}
				Row {
					anchors.horizontalCenter: parent.horizontalCenter
					spacing: 2
					MWLabel {
						id: altitudeLabel
						anchors.top: parent.top
						text: formatAltitude(compass.currentAltitude)
						font.pointSize: 12
					}
					MWLabel {
						anchors.top: parent.top
						text: altitudeLabel.text ? qsTr("m") : "";
						font.family: "MetaWatch Small caps 8pt"
						font.pointSize: 6
					}
				}
			}
		}
	}
}
