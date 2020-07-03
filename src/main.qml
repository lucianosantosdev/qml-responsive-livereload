import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 1.4

Window {
    visible: false
    width: 640
    height: 480

    Loader {
        id: _loader

        function reload() {
            source = "";
            $QmlEngine.clearCache();
            source = $QmlEngine.mainUrl();
        }

        anchors.centerIn: parent
        anchors.fill: parent
        source: $QmlEngine.mainUrl()
    }

    Connections {
        target: $QmlEngine
        onRefresh: {
            _loader.reload();
        }
    }
}
