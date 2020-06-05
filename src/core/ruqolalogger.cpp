/*
   Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "ruqolalogger.h"
#include "ruqola_debug.h"
#include <QCoreApplication>
#include <QFileInfo>
#include <QDateTime>

RuqolaLogger::RuqolaLogger(const QString &accountName)
    : RocketChatRestApi::AbstractLogger()
{
    static quint64 nextIdentifier = 0;
    mIdentifier = ++nextIdentifier;

    const QString filename = QLatin1String(qgetenv("RUQOLA_LOGFILE"))
            + QLatin1Char('-') + accountName
            + QLatin1Char('.')
            + QString::number(QCoreApplication::applicationPid())
            + QLatin1Char('.')
            + QString::number(mIdentifier);

    mLoggerFile.setFileName(filename);
    if (!mLoggerFile.open(QFile::WriteOnly)) {
        qCWarning(RUQOLA_LOG) << "Could not open log file for writing:" << mLoggerFile.fileName();
    }

    mRestApiLogFile.setFileName(QStringLiteral("RESTAPI") + filename);
    if (!mRestApiLogFile.open(QFile::WriteOnly)) {
        qCWarning(RUQOLA_LOG) << "Could not open log file for restapi:" << mRestApiLogFile.fileName();
    }

    //No convert to qCDebug as we want to see this info.
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
    switch(type) {
    case Unknown:
    case DDPType:
        break;
    case RESTApiType: {
        mRestApiLogFile.write('[' + QDateTime::currentDateTime().toString().toUtf8() + "] " + data + '\n');
        mRestApiLogFile.flush();
        break;
    }
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
