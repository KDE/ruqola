import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import "components"

Window {
    id: root

    width: 800
    height: 600

    visible: true
    title: "Ruqola"

    property var _iconsModelQml: null

    RowLayout {
        anchors.fill: parent
        spacing: 0

        ServerSelector {
            id: serverSelector
            Layout.fillHeight: true
            model: _serverSelectorController.count
        }

        // TODO: consider using stacklayout + dynamic loading & caching
        Loader {
            source: _screenController.loaderUrl
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Repeater {
            model: _iconsModel
            Item {
                Component.onCompleted: root._iconsModelQml = model;
            }
        }
    }
}
