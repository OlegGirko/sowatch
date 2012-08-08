import QtQuick 1.1
import com.nokia.meego 1.1
import com.nokia.extras 1.1

Page {
	id: watchPage
	anchors.leftMargin: UiConstants.DefaultMargin
	anchors.rightMargin: UiConstants.DefaultMargin

	tools: ToolBarLayout {
		ToolIcon {
			platformIconId: "toolbar-back"
			anchors.left: parent.left
			onClicked: pageStack.pop()
		}
	}

	ListView {
		id: emptyListView
		anchors.fill: parent
		model: ListModel {

		}

		delegate: ListDelegate {
			Image {
                source: "image://theme/icon-m-common-drilldown-arrow" + (theme.inverted ? "-inverse" : "")
                anchors.right: parent.right;
                anchors.verticalCenter: parent.verticalCenter
            }
		}
	}
	ScrollDecorator {
		flickableItem: watchesListView
	}
}
