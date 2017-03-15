import QtQuick 2.7
import QtQuick.Controls 2.1

Rectangle {
	id: root
	width: 400
	height: 400

	signal updateParameters(int range_min, int range_max)

	property var absAngleLimit: 45
	property var absOffsetLimit: 0.75

	Rectangle {
		anchors.fill: parent
		anchors.margins: 10

		Label {
			id: title
			anchors { left: parent.left; right: parent.right; top: parent.top; }
			text: "Thermal image settings"
			font.pixelSize: 20
		}

		Column {
			id: options
			anchors { top: title.bottom; left: parent.left; right: parent.right; bottom: parent.bottom }
			anchors.topMargin: 10

			spacing: 2

			LabeledRangeSlider {
				id: temp_range
				text: "Range"
				from: 0
				to: 1 << 14 // 2^14
				first.value: 8500
				second.value: 10000
			}

			Row {
				anchors.horizontalCenter: parent.horizontalCenter
				spacing: 2
				Button {
					text: "Update"
					onClicked: {
						root.updateParameters(temp_range.first.value, temp_range.second.value)
					}
				}
				Button {
					text: "Reset"
					onClicked: {
						temp_range.first.value = 8500
						temp_range.second.value = 10000
						root.updateParameters(temp_range.first.value, temp_range.second.value)
					}
				}
			}
		}
	}
}
