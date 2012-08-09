import QtQuick 1.1
import com.nokia.meego 1.1

Item {
	id: header
	height: 40

	property alias text: headerLabel.text

	Text {
		id: headerLabel
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		anchors.rightMargin: 8
		anchors.bottomMargin: 2
		font: UiConstants.GroupHeaderFont
		color: theme.inverted ? "#4D4D4D" : "#3C3C3C";
	}
	Image {
		anchors.right: headerLabel.left
		anchors.left: parent.left
		anchors.verticalCenter: headerLabel.verticalCenter
		anchors.rightMargin: 24
		source: "image://theme/meegotouch-groupheader" + (theme.inverted ? "-inverted" : "") + "-background"
	}
}
