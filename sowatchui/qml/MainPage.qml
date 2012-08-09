import QtQuick 1.1
import com.nokia.meego 1.1
import com.nokia.extras 1.1

Page {
	id: mainPage
	anchors.leftMargin: UiConstants.DefaultMargin
	anchors.rightMargin: UiConstants.DefaultMargin
	orientationLock: PageOrientation.LockPortrait

	tools: ToolBarLayout {
		ToolIcon {
			platformIconId: "toolbar-add"
			anchors.right: parent.right
			onClicked: newWatchSheet.open()
		}
	}

	NewWatchSheet {
		id: newWatchSheet
	}

	ListView {
		id: watchesListView
		anchors.fill: parent
		model: watches

		delegate: ListDelegate {
			Image {
                source: "image://theme/icon-m-common-drilldown-arrow" + (theme.inverted ? "-inverse" : "")
                anchors.right: parent.right;
                anchors.verticalCenter: parent.verticalCenter
            }

			onClicked: {
				var page = Qt.createComponent("WatchPage.qml");
				pageStack.push(page);
			}
		}
	}
	ScrollDecorator {
		flickableItem: watchesListView
	}
}
