/*
   Copyright (c) 2017-2019 Montel Laurent <montel@kde.org>

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

RuqolaLogger::RuqolaLogger(const QString &accountName)
    : RocketChatRestApi::AbstractLogger()
{
    static quint64 nextIdentifier = 0;
    mIdentifier = ++nextIdentifier;

    mFile.setFileName(QLatin1String(qgetenv("RUQOLA_LOGFILE"))
                      + QLatin1Char('-') + accountName
                      + QLatin1Char('.')
                      + QString::number(QCoreApplication::applicationPid())
                      + QLatin1Char('.')
                      + QString::number(mIdentifier));
    if (!mFile.open(QFile::WriteOnly)) {
        qCWarning(RUQOLA_LOG) << "Could not open log file for writing:" << mFile.fileName();
    }
    //No convert to qCDebug as we want to see this info.
    qDebug() << "Log in file: " << QFileInfo(mFile.fileName()).absoluteFilePath();
}

RuqolaLogger::~RuqolaLogger()
{
    mFile.close();
}

void RuqolaLogger::dataSent(const QByteArray &data)
{
    mFile.write("Sent: " + data.trimmed() + '\n');
    mFile.flush();
}

void RuqolaLogger::dataReceived(const QByteArray &data)
{
    mFile.write("Received: " + data.trimmed() + '\n');
    mFile.flush();
}
