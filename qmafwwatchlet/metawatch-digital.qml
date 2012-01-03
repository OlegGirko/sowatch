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

		Text {
			anchors.fill: parent
			text: player.title.length > 0 ? player.title : qsTr("No media");
			color: "black"
		}
	}

	Connections {
		target: watch
		onButtonPressed : {
			switch(button) {
			case 1:
				player.volumeUp();
				break;
			case 2:
				player.volumeDown();
				break;
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
