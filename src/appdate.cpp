// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

#include "appdate.h"

AppDate::AppDatePtr AppDate::instance() {
    static AppDatePtr appDate(new AppDate);

    return appDate;
}

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
