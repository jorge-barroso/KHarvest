#include "addedtasksmodel.h"
#include "addedtaskslist.h"

AddedTasksModel::AddedTasksModel(QObject *parent)
        : QAbstractListModel(parent), mList(nullptr) {}

int AddedTasksModel::rowCount(const QModelIndex &parent) const {
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !mList)
        return 0;

    return mList->tasks().size();
}

QVariant AddedTasksModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || !mList)
        return {};

    Task *const pTask = mList->tasks().at(index.row());
    switch (role) {
        case HeaderRole:
            return {pTask->get_project_label()};
        case SubtitleRole:
            return {pTask->taskName};
        case NoteRole:
            return {pTask->note};
        case TimeLabelRole:
            return {pTask->timeTracked.toString("hh:mm")};
        case TimeRole:
            return {pTask->timeTracked};
        case ProjectNameRole:
            return {pTask->projectName};
        case ShouldAutoStopRole:
            return {pTask->shouldAutomaticallyStop};
        case StartedRole:
            return {pTask->started};
        default:
            return {};
    }
}

bool AddedTasksModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!mList)
        return false;

    Task *task{mList->tasks().at(index.row())};
    switch (role) {
        case HeaderRole:
            task->update_task_from_project_label(value.toString());
            break;
        case SubtitleRole:
            task->taskName = value.toString();
            break;
        case NoteRole:
            task->note = value.toString();
            break;
        case TimeLabelRole:
            task->timeTracked = QTime::fromString(value.toString(), "hh:mm");
            break;
        case TimeRole:
            task->timeTracked = value.toTime();
            emit dataChanged(index, index, {TimeLabelRole});
            break;
        case ProjectNameRole:
            task->projectName = value.toString();
            break;
        case ShouldAutoStopRole:
            task->shouldAutomaticallyStop = value.toBool();
            break;
        case StartedRole:
            if (value.toBool()) {
                mList->startTask(index.row());
            } else {
                mList->stopTask(index.row());
            }
            break;
        default:
            return false;
    }

    if (mList->taskEdited(index.row(), task)) {
        return false;
    }

    emit dataChanged(index, index, {role});
    return true;
}

Qt::ItemFlags AddedTasksModel::flags(const QModelIndex &index) const {
    return index.isValid() ? Qt::ItemIsEditable : Qt::NoItemFlags;
}

QHash<int, QByteArray> AddedTasksModel::roleNames() const {
    return {
            {HeaderRole,         "header"},
            {SubtitleRole,       "subtitle"},
            {NoteRole,           "note"},
            {TimeLabelRole,      "timeLabel"},
            {TimeRole,           "time"},
            {ProjectNameRole,    "project"},
            {ShouldAutoStopRole, "shouldAutomaticallyStop"},
            {StartedRole,        "started"},
    };
}

AddedTasksList *AddedTasksModel::list() const {
    return mList;
}

void AddedTasksModel::setList(AddedTasksList *list) {
    beginResetModel();

    if (mList)
        mList->disconnect(this);

    mList = list;

    if (mList != nullptr) {
        connect(mList, &AddedTasksList::preTaskAdded, this, [this]() {
            const int index = mList->tasks().size();
            beginInsertRows(QModelIndex(), index, index);
        });
        connect(mList, &AddedTasksList::postTaskAdded, this, [this]() {
            endInsertRows();
        });
        connect(mList, &AddedTasksList::preTaskRemoved, this, [this](const int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });
        connect(mList, &AddedTasksList::postTaskRemoved, this, [this]() {
            endRemoveRows();
        });
    }

    endResetModel();
}
