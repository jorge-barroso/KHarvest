// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

#include "task.h"

const QString Task::values_separator = "~~harvest~~";

const QString newline('\n');
const QString cr('\r');

#ifdef __linux__
const QString Task::end_line_separator = newline;
#elif _WIN32
const QString Task::end_line_separator = cr + newline;
#else
const QString Task::end_line_separator = cr;
#endif

const QString Task::PROJECT_ID_KEY{"project_id"};
const QString Task::TASK_ID_KEY{"task_id"};
const QString Task::TIME_ENTRY_ID_KEY{"time_entry_id"};
const QString Task::CLIENT_NAME_KEY{"client_name"};
const QString Task::PROJECT_NAME_KEY{"project_name"};
const QString Task::TASK_NAME_KEY{"task_name"};
const QString Task::TIME_TRACKED_KEY{"time_tracked"};
const QString Task::NOTE_KEY{"note"};
const QString Task::STARTED_KEY{"started"};
const QString Task::DATE_KEY{"date"};