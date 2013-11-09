import Qt 4.7
import com.javispedro.sowatch 1.0

Rectangle {
	width: 96
	height: time.height + (title.visible ? 16 : 0)

	property alias text: title.text
	property alias font: title.font
	property alias icon: image

	clip: true

	Image {
		id: image
		anchors.top: time.bottom
		anchors.left: parent.left
	}

	MWLabel {
		id: title
		anchors.top: time.bottom
		anchors.left: image.right
		anchors.leftMargin: 2
		font.pixelSize: 16
		visible: text != ""
	}

	MWLabel {
		id: status
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.leftMargin: 1
		height: 8
		font.family: "MetaWatch Large caps 8pt"
		font.pixelSize: 8
		text: ""
	}

	MWLabel {
		id: time
		anchors.right: parent.right
		anchors.rightMargin: 1
		anchors.top: parent.top
		height: 8
		font.family: "MetaWatch Large caps 8pt"
		font.pixelSize: 8
		text: "  :  "
	}

	function updateStatusBar() {
		var now = new Date
		time.text = Qt.formatDate(now, "ddd") + " " + Qt.formatTime(now)
		status.text = notifications.fullCount() > 0 ? "*" : ""
	}

	Timer {
		triggeredOnStart: true
		interval: 100000
		repeat: true
		running: watch.active
		onTriggered: updateStatusBar()
	}
}
