import QtQuick 2.7
import QtQuick.Controls 2.1

Rectangle {
	id: root

	property var absAngleLimit: 45.0
	
	Rectangle {
		anchors.fill: parent
		anchors.margins: 10

		Label {
			id: title
			anchors { left: parent.left; right: parent.right; top: parent.top; }
			text: "Projection parameters"
			font.pixelSize: 20
		}

		Column {
			id: options
			anchors { top: title.bottom; left: parent.left; right: parent.right; bottom: parent.bottom }
			anchors.topMargin: 10

			spacing: 2

			LabeledSlider {
				id: pitch
				text: "Pitch"
				from: -root.absAngleLimit
				to: root.absAngleLimit
				value: 0.0
			}
			LabeledSlider {
				id: yaw
				text: "Yaw"
				from: -root.absAngleLimit
				to: root.absAngleLimit
				value: 0.0
			}
		}
	}
}
