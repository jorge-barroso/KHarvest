//
// Created by jorge on 21/02/23.
//

#include "errorhandler.h"

ErrorHandler::ErrorHandlerPointer ErrorHandler::instance() {
    static ErrorHandlerPointer errorHandler(new ErrorHandler);

    return errorHandler;
}

void ErrorHandler::sendError(const QString &errorText) {
    emit error(errorText);
}

void ErrorHandler::sendWarning(const QString &warningText) {
    emit warning(warningText);
}

ErrorHandler::ErrorHandler() : QObject() {}

