import QtQuick 1.0
import com.javispedro.sowatch.metawatch 1.0

Rectangle {
	width: 96
	height: 96

	color: "white"

	MWTitle {
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
		visible: mediaart.status != Image.Ready
		text: qsTr("Now playing")
		icon.source: "icon.png"
	}

	Image {
		id: mediaart
		anchors.fill: parent
		fillMode: Image.PreserveAspectFit
		smooth: true
		source: player.mediaArt
	}

	Rectangle {
		x: 0
		width: parent.width
		height: 14
		anchors.bottom: parent.bottom
		color: "white"

		MWLabel {
			anchors.fill: parent
			text: player.title.length > 0 ? player.title : qsTr("No media");
			color: "black"
		}
	}

	Rectangle {
		id: volumeBar
		width: 18

		anchors.top: parent.top
		anchors.bottom: parent.bottom
		anchors.right: parent.right

		color: "white"

		visible: false

		Image {
			id: volumeIcon
			anchors.top: parent.top
			anchors.left: parent.left
			anchors.leftMargin: 2

			source: "volume.png"
		}

		Rectangle {
			id: volumeBarBox

			anchors.top: volumeIcon.bottom
			anchors.left: parent.left
			anchors.right: parent.right
			anchors.bottom: parent.bottom
			anchors.leftMargin: 2

			color: "black"

			Rectangle {
				id: volumeBarThing

				anchors.left: parent.left
				anchors.right: parent.right
				anchors.bottom: parent.bottom
				anchors.margins: 4

				height: volumeBar.visible ?
					(volumeControl.volume * (parent.height - anchors.margins*2)) / (volumeControl.max - volumeControl.min) :
					0; // Avoid unnecessary updates when not visible.

				color: "white"
			}
		}

		Timer {
			id: autoHideTimer
			interval: 3000
			repeat: false
			onTriggered: {
				volumeBar.visible = false;
			}
		}

		function show() {
			volumeBar.visible = true;
			autoHideTimer.restart();
		}
	}

	Connections {
		target: watch
		onButtonPressed : {
			switch(button) {
			case 1: {
				volumeControl.up();
				volumeBar.show();
				break;
			}
			case 2: {
				volumeControl.down();
				volumeBar.show();
				break;
			}
			case 4:
				player.playPause();
				break;
			case 5:
				player.next();
				break;
			}
		}
	}
}
