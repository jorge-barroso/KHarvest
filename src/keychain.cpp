// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

#include "keychain.h"

#include <QDebug>
#include <QEventLoop>

KeyChain::KeyChain(QObject *parent)
        : QObject(parent), mReadCredentialJob(QLatin1String("org.kde.kharvest")),
          mWriteCredentialJob(QLatin1String("org.kde.kharvest")),
          mDeleteCredentialJob(QLatin1String("org.kde.kharvest")) {
    mReadCredentialJob.setAutoDelete(false);
    mWriteCredentialJob.setAutoDelete(false);
    mDeleteCredentialJob.setAutoDelete(false);
}

void KeyChain::readKey(const QString &key) {
    mReadCredentialJob.setKey(key);

    connect(&mReadCredentialJob, &QKeychain::ReadPasswordJob::finished, this, [this, key]() {
        if (mReadCredentialJob.error()) {
            auto errorMessage = QString("%1/%2 key read failed: %3")
                    .arg(mReadCredentialJob.service(),
                         mReadCredentialJob.key(),
                         qPrintable(mReadCredentialJob.errorString()));

            qDebug() << errorMessage;
            emit error(errorMessage);
        } else {
            qDebug() << QString("%1/%2 key read succeeded").arg(mReadCredentialJob.service(),
                                                                mReadCredentialJob.key());
            emit keyRestored(key, mReadCredentialJob.textData());
        }
    });

    mReadCredentialJob.start();
}

QString KeyChain::readKeySynchronous(const QString &key) {
    QString value{""};

    QEventLoop loop;
    connect(this, &KeyChain::keyRestored, this, [&](const QString &, const QString &textData) {
        value = textData;
    });
    connect(this, &KeyChain::error, &loop, &QEventLoop::quit);
    connect(this, &KeyChain::keyRestored, &loop, &QEventLoop::quit);

    readKey(key);
    loop.exec();

    return value;
}

void KeyChain::writeKey(const QString &key, const QString &value) {
    mWriteCredentialJob.setKey(key);

    connect(&mWriteCredentialJob, &QKeychain::WritePasswordJob::finished, this, [this, key]() {
        if (mWriteCredentialJob.error()) {
            auto errorMessage{QString("%1/%2 key write failed: %3")
                                      .arg(mWriteCredentialJob.service(),
                                           mWriteCredentialJob.key(),
                                           qPrintable(mWriteCredentialJob.errorString()))};

            qDebug() << errorMessage;
            emit error(errorMessage);
        } else {
            emit keyStored(key);
        }
    });

    mWriteCredentialJob.setTextData(value);
    mWriteCredentialJob.start();
}

void KeyChain::deleteKey(const QString &key) {
    mDeleteCredentialJob.setKey(key);

    connect(&mDeleteCredentialJob, &QKeychain::DeletePasswordJob::finished, this, [this, key]() {
        if (mDeleteCredentialJob.error()) {
            auto errorMessage{QString("%1/%2 key delete failed: %3")
                                      .arg(mDeleteCredentialJob.service(),
                                           mDeleteCredentialJob.key(),
                                           qPrintable(mDeleteCredentialJob.errorString()))};
            qDebug() << errorMessage;
            emit error(errorMessage);
        } else {
            emit keyDeleted(key);
        }
    });

    mDeleteCredentialJob.start();
}
