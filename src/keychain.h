//
// Created by jorge on 10/02/23.
//

#ifndef KHARVEST_KEYCHAIN_H
#define KHARVEST_KEYCHAIN_H


#include <QObject>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

#include <qt5keychain/keychain.h>

#else
#include <qt6keychain/keychain.h>
#endif

class KeyChain : public QObject {
Q_OBJECT
public:
    explicit KeyChain(QObject *parent = nullptr);

    void readKey(const QString &key);

    QString readKeySynchronous(const QString &key);

    void writeKey(const QString &key, const QString &value);

    void deleteKey(const QString &key);

signals:

    void keyStored(const QString &key);

    void keyRestored(const QString &key, const QString &value);

    void keyDeleted(const QString &key);

    void error(const QString &errorText);

private:
    QKeychain::ReadPasswordJob mReadCredentialJob;
    QKeychain::WritePasswordJob mWriteCredentialJob;
    QKeychain::DeletePasswordJob mDeleteCredentialJob;
};


#endif //KHARVEST_KEYCHAIN_H
