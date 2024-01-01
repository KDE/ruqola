/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QObject>
#include <QSslError>
class QNetworkAccessManager;
class QNetworkReply;
class LIBRUQOLACORE_EXPORT DownloadAvartarUrlManager : public QObject
{
    Q_OBJECT
public:
    explicit DownloadAvartarUrlManager(QObject *parent = nullptr);
    ~DownloadAvartarUrlManager() override;

    [[nodiscard]] bool canStart() const;
    void start();

    [[nodiscard]] QString avatarUrl() const;
    void setAvatarUrl(const QString &avatarUrl);
Q_SIGNALS:
    void avatarDownloaded(const QByteArray &data, const QString &avatarUrl);

private:
    LIBRUQOLACORE_NO_EXPORT void slotFileDownloaded(QNetworkReply *reply);
    LIBRUQOLACORE_NO_EXPORT void slotSslErrors(QNetworkReply *reply, const QList<QSslError> &error);
    QString mAvatarUrl;
    QNetworkAccessManager *const mNetworkAccessManager;
};
