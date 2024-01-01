/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] QString translatedString(const QString &language, const QString &id);
    void parse(const QString &serverUrl);

    [[nodiscard]] bool fileParsed() const;

    [[nodiscard]] QString accountName() const;
    void setAccountName(const QString &accountName);

    [[nodiscard]] QString serverVersion() const;
    void setServerVersion(const QString &serverVersion);

Q_SIGNALS:
    void fileLanguagesParseSuccess();
    void fileLanguagesParseFailed();

private:
    [[nodiscard]] QString storedFileName() const;
    LIBRUQOLACORE_NO_EXPORT void slotFileDownloaded(const QByteArray &data);
    LIBRUQOLACORE_NO_EXPORT void parseLanguageFile();
    QString mAccountName;
    QString mServerVersion;
    //<id, DownloadAppsLanguagesInfo>
    QMap<QString, DownloadAppsLanguagesInfo> mLanguageMap;
    bool mFileParsed = false;
};
