import QtQuick
import qs.Commons
import qs.Ui

BarWidget {
  id: root
  moduleName: "papaya.logo"

  implicitWidth: button.implicitWidth
  implicitHeight: button.implicitHeight

  WidgetButton {
    id: button
    anchors.fill: parent
    bar: root.bar
    text: ""
    labelVisible: false
    hasVisualContent: true
    horizontalMargin: 7.5
    fixedWidth: Style.spaceReal(20) + Style.spaceReal(horizontalMargin) * 2
    tooltipText: ""

    onPressed: function(mouseButton) {
      if (!root.bar) return
      if (mouseButton === Qt.RightButton) root.bar.run("xdg-terminal-exec")
      else root.bar.run("omarchy-shell shell toggle omarchy.menu '{\"menu\":\"root\"}'")
    }

    Image {
      anchors.centerIn: parent
      width: Style.space(20)
      height: Style.space(20)
      source: Qt.resolvedUrl("mclaren.svg")
      fillMode: Image.PreserveAspectFit
      smooth: true
      mipmap: true
      sourceSize.width: Math.round(width * 2)
      sourceSize.height: Math.round(height * 2)
    }
  }
}
