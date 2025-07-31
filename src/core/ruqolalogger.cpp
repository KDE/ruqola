/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolalogger.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqola_debug.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QFileInfo>

RuqolaLogger::RuqolaLogger(const QString &accountName)

{
    static quint64 nextIdentifier = 0;
    mIdentifier = ++nextIdentifier;

    const QString filename = QLatin1StringView(qgetenv("RUQOLA_LOGFILE")) + u'-' + accountName + u'.' + QString::number(QCoreApplication::applicationPid())
        + u'.' + QString::number(mIdentifier);

    mLoggerFile.setFileName(filename);
    if (!mLoggerFile.open(QFile::WriteOnly)) {
        qCWarning(RUQOLA_LOG) << "Could not open log file for writing:" << mLoggerFile.fileName();
    }

    mRestApiLogFile.setFileName(u"RESTAPI_"_s + filename);
    if (!mRestApiLogFile.open(QFile::WriteOnly)) {
        qCWarning(RUQOLA_LOG) << "Could not open log file for restapi:" << mRestApiLogFile.fileName();
    }

    mDatabaseLogFile.setFileName(u"DATABASE_"_s + filename);
    if (!mDatabaseLogFile.open(QFile::WriteOnly)) {
        qCWarning(RUQOLA_LOG) << "Could not open log file for database:" << mDatabaseLogFile.fileName();
    }

    // No convert to qCDebug as we want to see this info.
    qDebug() << "Log in file: " << loggerFilePath();
    qDebug() << "Log send RESTAPI in file: " << restApiLoggerFilePath();
    qDebug() << "Log database info in file: " << restApiLoggerFilePath();
}

RuqolaLogger::~RuqolaLogger()
{
    mLoggerFile.close();
    mRestApiLogFile.close();
    mDatabaseLogFile.close();
}

QString RuqolaLogger::databaseLogFilePath() const
{
    return QFileInfo(mDatabaseLogFile.fileName()).absoluteFilePath();
}

QString RuqolaLogger::loggerFilePath() const
{
    return QFileInfo(mLoggerFile.fileName()).absoluteFilePath();
}

QString RuqolaLogger::restApiLoggerFilePath() const
{
    return QFileInfo(mRestApiLogFile.fileName()).absoluteFilePath();
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
    mLoggerFile.write('[' + QDateTime::currentDateTime().toString().toUtf8() + "] Received: " + data.trimmed() + '\n');
    mLoggerFile.flush();
}

void RuqolaLogger::dataLoadFromDatabase(const QByteArray &data)
{
    mDatabaseLogFile.write('[' + QDateTime::currentDateTime().toString().toUtf8() + "] Load: " + data.trimmed() + '\n');
    mLoggerFile.flush();
}

void RuqolaLogger::dataSaveFromDatabase(const QByteArray &data)
{
    mDatabaseLogFile.write("Save: " + data.trimmed() + '\n');
    mLoggerFile.flush();
}
