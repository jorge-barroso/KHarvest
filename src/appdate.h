#ifndef KHARVEST_APPDATE_H
#define KHARVEST_APPDATE_H


#include <QObject>
#include <QDateTime>

class AppDate : public QObject {
Q_OBJECT
    Q_PROPERTY(QDate date
                       READ date
                       WRITE setDate
                       NOTIFY dateChanged)
public:
    explicit AppDate(QObject *parent = nullptr);

    [[nodiscard]] QDate date() const;

    void setDate(const QDate &newDate);

signals:

    void dateChanged();

public slots:

    void addDays(int days);

private:
    QDate mDate;
};


#endif //KHARVEST_APPDATE_H
