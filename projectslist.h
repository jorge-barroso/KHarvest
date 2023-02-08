#ifndef PROJECTSLIST_H
#define PROJECTSLIST_H

#include <QObject>

class ProjectsList : public QObject
{
    Q_OBJECT
public:
    explicit ProjectsList(QObject *parent = nullptr);

signals:

};

#endif // PROJECTSLIST_H
