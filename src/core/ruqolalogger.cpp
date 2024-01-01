/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolalogger.h"
#include "ruqola_debug.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QFileInfo>

RuqolaLogger::RuqolaLogger(const QString &accountName)
    : RocketChatRestApi::AbstractLogger()
{
    static quint64 nextIdentifier = 0;
    mIdentifier = ++nextIdentifier;

    const QString filename = QLatin1String(qgetenv("RUQOLA_LOGFILE")) + QLatin1Char('-') + accountName + QLatin1Char('.')
        + QString::number(QCoreApplication::applicationPid()) + QLatin1Char('.') + QString::number(mIdentifier);

    mLoggerFile.setFileName(filename);
    if (!mLoggerFile.open(QFile::WriteOnly)) {
        qCWarning(RUQOLA_LOG) << "Could not open log file for writing:" << mLoggerFile.fileName();
    }

    mRestApiLogFile.setFileName(QStringLiteral("RESTAPI_") + filename);
    if (!mRestApiLogFile.open(QFile::WriteOnly)) {
        qCWarning(RUQOLA_LOG) << "Could not open log file for restapi:" << mRestApiLogFile.fileName();
    }

    // No convert to qCDebug as we want to see this info.
    qDebug() << "Log in file: " << QFileInfo(mLoggerFile.fileName()).absoluteFilePath();
    qDebug() << "Log send RESTAPI in file: " << QFileInfo(mRestApiLogFile.fileName()).absoluteFilePath();
}

RuqolaLogger::~RuqolaLogger()
{
    mLoggerFile.close();
    mRestApiLogFile.close();
}

void RuqolaLogger::dataSent(DataType type, const QByteArray &label, const QByteArray &data)
{
    switch (type) {
    case Unknown:
    case DDPType:
        break;
    case RESTApiType:
        mRestApiLogFile.write('[' + QDateTime::currentDateTime().toString().toUtf8() + "] " + data + '\n');
        mRestApiLogFile.flush();
        break;
    }
    dataSent(label + ' ' + data);
}

void RuqolaLogger::dataSent(const QByteArray &data)
{
    mLoggerFile.write("Sent: " + data.trimmed() + '\n');
    mLoggerFile.flush();
}

void RuqolaLogger::dataReceived(const QByteArray &data)
{
    mLoggerFile.write("Received: " + data.trimmed() + '\n');
    mLoggerFile.flush();
}
