/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDate>
#include <QObject>
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT PreviewUrlCacheManager : public QObject
{
    Q_OBJECT
public:
    explicit PreviewUrlCacheManager(RocketChatAccount *account, QObject *parent = nullptr);
    ~PreviewUrlCacheManager() override;

    [[nodiscard]] int embedCacheExpirationDays() const;
    void setEmbedCacheExpirationDays(int newEmbedCacheExpirationDays);

    [[nodiscard]] QString cachePath() const;
    void setCachePath(const QString &newCachePath);

    [[nodiscard]] QDate currentDate() const;
    void setCurrentDate(const QDate &newCurrentDateTime);

private:
    LIBRUQOLACORE_NO_EXPORT void saveLastCheckedDateTime();
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT bool needToCheck() const;
    LIBRUQOLACORE_NO_EXPORT void checkCache();
    QDate mCurrentDate = QDate::currentDate();
    QString mCachePath;
    int mEmbedCacheExpirationDays = -1;
    RocketChatAccount *const mRocketChatAccount;
};
