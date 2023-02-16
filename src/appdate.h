// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

#ifndef KHARVEST_APPDATE_H
#define KHARVEST_APPDATE_H


#include <QObject>
#include <QDateTime>
#include <memory>

class AppDate : public QObject {
Q_OBJECT
    Q_PROPERTY(QDate date
                       READ date
                       WRITE setDate
                       NOTIFY dateChanged)
    using AppDatePtr = std::shared_ptr<AppDate>;
public:
    static AppDatePtr instance();

    [[nodiscard]] QDate date() const;

    void setDate(const QDate &newDate);

signals:

    void dateChanged();

public slots:

    void addDays(int days);

private:
    explicit AppDate(QObject *parent = nullptr);
    QDate mDate;
};


#endif //KHARVEST_APPDATE_H
