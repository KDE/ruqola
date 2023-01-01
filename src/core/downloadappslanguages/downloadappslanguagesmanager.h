/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "downloadappslanguagesinfo.h"
#include "libruqolacore_export.h"
#include <QMap>
#include <QObject>

class LIBRUQOLACORE_EXPORT DownloadAppsLanguagesManager : public QObject
{
    Q_OBJECT
public:
    explicit DownloadAppsLanguagesManager(QObject *parent = nullptr);
    ~DownloadAppsLanguagesManager() override;

    Q_REQUIRED_RESULT QString translatedString(const QString &language, const QString &id);
    void parse(const QString &serverUrl);

    Q_REQUIRED_RESULT bool fileParsed() const;

    Q_REQUIRED_RESULT QString accountName() const;
    void setAccountName(const QString &accountName);

    Q_REQUIRED_RESULT QString serverVersion() const;
    void setServerVersion(const QString &serverVersion);

Q_SIGNALS:
    void fileLanguagesParseSuccess();
    void fileLanguagesParseFailed();

private:
    Q_REQUIRED_RESULT QString storedFileName() const;
    void slotFileDownloaded(const QByteArray &data);
    void parseLanguageFile();
    QString mAccountName;
    QString mServerVersion;
    //<id, DownloadAppsLanguagesInfo>
    QMap<QString, DownloadAppsLanguagesInfo> mLanguageMap;
    bool mFileParsed = false;
};
