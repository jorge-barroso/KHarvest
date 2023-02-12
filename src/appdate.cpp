//
// Created by jorge on 12/02/23.
//

#include "appdate.h"

AppDate::AppDate(QObject *parent)
    : QObject(parent)
    , mDate{QDate::currentDate()} {}

QDate AppDate::date() const {
    return mDate;
}

void AppDate::setDate(const QDate &newDate) {
    this->mDate = newDate;
    emit dateChanged();
}

void AppDate::addDays(const int days) {
    this->mDate = this->mDate.addDays(days);
    emit dateChanged();
}
