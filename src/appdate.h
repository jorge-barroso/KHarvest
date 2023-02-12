#ifndef KHARVEST_APPDATE_H
#define KHARVEST_APPDATE_H


#include <QObject>
#include <QDateTime>

class AppDate : public QObject {
Q_OBJECT
    Q_PROPERTY(QDateTime date
                       READ date
                       WRITE setDate
                       NOTIFY dateChanged)
public:
    explicit AppDate(QObject *parent = nullptr);

    [[nodiscard]] QDateTime date() const;

    void setDate(const QDateTime &newDate);

signals:

    void dateChanged();

public slots:

    void addDays(int days);

private:
    QDateTime mDate;
};


#endif //KHARVEST_APPDATE_H
