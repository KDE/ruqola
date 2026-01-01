/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "downloadappslanguagesinfo.h"
#include "libruqolacore_export.h"
#include <QMap>
#include <QObject>
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT DownloadAppsLanguagesManager : public QObject
{
    Q_OBJECT
public:
    explicit DownloadAppsLanguagesManager(RocketChatAccount *account, QObject *parent = nullptr);
    ~DownloadAppsLanguagesManager() override;

    [[nodiscard]] QString translatedString(const QString &language, const QString &id);
    void parse();

    [[nodiscard]] QMap<QString, DownloadAppsLanguagesInfo> languagesAppsMap() const;

Q_SIGNALS:
    void fileLanguagesParseSuccess();
    void fileLanguagesParseFailed();

private:
    LIBRUQOLACORE_NO_EXPORT void slotFileDownloaded(const QJsonObject &json);
    //<id, DownloadAppsLanguagesInfo>
    QMap<QString, DownloadAppsLanguagesInfo> mLanguageMap;
    RocketChatAccount *const mRocketChatAccount;
};
