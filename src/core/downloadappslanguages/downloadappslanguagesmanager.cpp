/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "downloadappslanguagesmanager.h"
#include "downloadappslanguagesjob.h"
#include "downloadappslanguagesparser.h"
#include "managerdatapaths.h"
#include "ruqola_debug.h"
#include <QFileInfo>
#include <QStandardPaths>
#include <QTextStream>

DownloadAppsLanguagesManager::DownloadAppsLanguagesManager(QObject *parent)
    : QObject(parent)
{
}

DownloadAppsLanguagesManager::~DownloadAppsLanguagesManager()
{
}

QString DownloadAppsLanguagesManager::storedFileName() const
{
    return ManagerDataPaths::self()->path(ManagerDataPaths::Config, mAccountName) + QStringLiteral("/languages-%1.json").arg(mServerVersion);
}

void DownloadAppsLanguagesManager::parse(const QString &serverUrl)
{
    if (mFileParsed) {
        Q_EMIT fileLanguagesParseSuccess();
        return;
    }
    if (mAccountName.isEmpty()) {
        qCWarning(RUQOLA_LOG) << "account name is empty. It's a bug";
        Q_EMIT fileLanguagesParseFailed(); //Need to load commandlist
        return;
    }
    if (mServerVersion.isEmpty()) {
        qCWarning(RUQOLA_LOG) << "serverVersion is empty. It's a bug";
        Q_EMIT fileLanguagesParseFailed(); //Need to load commandlist
        return;
    }
    if (QFileInfo::exists(storedFileName())) {
        parseLanguageFile();
    } else {
        DownloadAppsLanguagesJob *job = new DownloadAppsLanguagesJob(this);
        job->setServerUrl(serverUrl);
        connect(job, &DownloadAppsLanguagesJob::fileDownloaded, this, &DownloadAppsLanguagesManager::slotFileDownloaded);
        job->start();
    }
}

void DownloadAppsLanguagesManager::slotFileDownloaded(const QByteArray &data)
{
    QFile f(storedFileName());
    if (f.open(QIODevice::WriteOnly)) {
        QTextStream out(&f);
        out << data;
        f.close();
    }
    parseLanguageFile();
}

void DownloadAppsLanguagesManager::parseLanguageFile()
{
    DownloadAppsLanguagesParser parser;
    parser.setFilename(storedFileName());
    parser.parse();
    mLanguageMap = parser.map();
    qDebug() << "mLanguageMap " << mLanguageMap.count();
    mFileParsed = true;
    Q_EMIT fileLanguagesParseSuccess();
}

QString DownloadAppsLanguagesManager::serverVersion() const
{
    return mServerVersion;
}

void DownloadAppsLanguagesManager::setServerVersion(const QString &serverVersion)
{
    mServerVersion = serverVersion;
}

QString DownloadAppsLanguagesManager::accountName() const
{
    return mAccountName;
}

void DownloadAppsLanguagesManager::setAccountName(const QString &accountName)
{
    mAccountName = accountName;
}

bool DownloadAppsLanguagesManager::fileParsed() const
{
    return mFileParsed;
}

void DownloadAppsLanguagesManager::translatedString(const QString &language, const QString &appId)
{
    if (!mFileParsed) {
        qCWarning(RUQOLA_LOG) << "language file is not parsed yet!";
        return;
    }
    //TODO
}
