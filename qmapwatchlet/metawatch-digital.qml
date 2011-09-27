import QtQuick 1.0
import QtMobility.location 1.2

Rectangle {
	width: 96
	height: 96

	color: "white"

	PositionSource {
		id: pos
		updateInterval: 5000
		active: watch.active
	}

	Map {
		id: map
		anchors.fill: parent;
		plugin : Plugin { name : "nokia" }
		center: pos.position.coordinate
	}

	Connections {
		target: watch
		onButtonPressed : {
			switch(button) {
			case 1:
				map.zoomLevel -= 1;
				break;
			case 2:
				map.zoomLevel += 1;
				break;
			}
		}
	}
}
