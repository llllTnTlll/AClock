import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "qrc:/widgets"
import helper


Rectangle{
    anchors.fill: parent
    color: "#2d2d2d"

    ColumnLayout{
        anchors.fill: parent
        spacing: 0

        Rectangle{
            color: "#272727"
            Layout.fillWidth: parent
            Layout.preferredHeight: parent.height * 0.85

            RowLayout{
                anchors.fill: parent
                spacing: 0

                CpuWatcher{
                    onCurrentCpuUsageChanged: {
                        cpuGauges.setPercentage(currentCpuUsage);
                    }
                }

                Rectangle{
                    color: "#272727"
                    Layout.fillHeight: parent
                    Layout.preferredWidth: parent.width * 0.33
                    Gauges{
                        id: cpuGauges
                        gaugesHeight: 200
                        gaugesWidth: 200
                        text: 'CPU'

                        progressPercent: 40
                    }
                }
                Rectangle{
                    color: "#272727"
                    Layout.fillHeight: parent
                    Layout.preferredWidth: parent.width * 0.33
                    Gauges{
                        gaugesHeight: 200
                        gaugesWidth: 200
                        text: 'GPU'
                    }
                }
                Rectangle{
                    color: "#272727"
                    Layout.fillHeight: parent
                    Layout.preferredWidth: parent.width * 0.33
                    Gauges{
                        gaugesHeight: 200
                        gaugesWidth: 200
                        text: 'MEM'
                    }
                }
            }
        }
        Rectangle{
            color: "#1a1a1a"
            Layout.fillWidth: parent
            Layout.preferredHeight: parent.height * 0.15
        }
    }

}
