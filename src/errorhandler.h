//
// Created by jorge on 21/02/23.
//

#ifndef KHARVEST_ERRORHANDLER_H
#define KHARVEST_ERRORHANDLER_H


#include <QObject>
#include <memory>

class ErrorHandler : public QObject {
Q_OBJECT
typedef std::shared_ptr<ErrorHandler> ErrorHandlerPointer;
public:
    static ErrorHandlerPointer instance();
    void sendError(const QString& errorText);
    void sendWarning(const QString& warningText);
signals:
    void error(const QString& errorText);
    void warning(const QString& errorText);
private:
public:
    explicit ErrorHandler();
};


#endif //KHARVEST_ERRORHANDLER_H
