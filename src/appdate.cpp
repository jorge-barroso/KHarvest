//
// Created by jorge on 12/02/23.
//

#include "appdate.h"

#include <utility>

AppDate::AppDate(QObject *parent)
    : QObject(parent)
    , mDate{QDateTime::currentDateTime()} {}

QDateTime AppDate::date() const {
    return mDate;
}

void AppDate::setDate(const QDateTime &newDate) {
    this->mDate = newDate;
    emit dateChanged();
}

void AppDate::addDays(const int days) {
    this->mDate = this->mDate.addDays(days);
    emit dateChanged();
}
