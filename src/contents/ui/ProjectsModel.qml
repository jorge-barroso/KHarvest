// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>

import QtQuick 2.9
import QtQuick.Controls 2.3 as Controls
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.19 as Kirigami

ListModel{
    id: projectsModel
    ListElement {
        projectName: "Test Project 1"
        subitems: [
            ListElement {taskName: "Task 1"},
            ListElement {taskName: "Task 2"},
            ListElement {taskName: "Task 3"}
        ]
    }
    ListElement {
        projectName: "Test Project 2"
        subitems: [
            ListElement {taskName: "Task 4"},
            ListElement {taskName: "Task 5"},
            ListElement {taskName: "Task 6"}
        ]
    }
    ListElement {
        projectName: "Test Project 3"
        subitems: [
            ListElement {taskName: "Task 7"},
            ListElement {taskName: "Task 8"},
            ListElement {taskName: "Task 9"}
        ]
    }
    ListElement {
        projectName: "Test Project 4"
        subitems: [
            ListElement {taskName: "Task 10"},
            ListElement {taskName: "Task 11"},
            ListElement {taskName: "Task 12"}
        ]
    }
    ListElement {
        projectName: "Test Project 5"
        subitems: [
            ListElement {taskName: "Task 13"},
            ListElement {taskName: "Task 14"},
            ListElement {taskName: "Task 15"}
        ]
    }
    ListElement {
        projectName: "Test Project 6"
        subitems: [
            ListElement {taskName: "Task 16"},
            ListElement {taskName: "Task 17"},
            ListElement {taskName: "Task 18"}
        ]
    }
}
