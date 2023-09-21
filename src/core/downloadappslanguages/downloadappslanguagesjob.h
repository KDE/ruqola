/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QObject>
#include <QSslError>
class QNetworkAccessManager;
class QNetworkReply;
class LIBRUQOLACORE_EXPORT DownloadAppsLanguagesJob : public QObject
{
    Q_OBJECT
public:
    explicit DownloadAppsLanguagesJob(QObject *parent = nullptr);
    ~DownloadAppsLanguagesJob() override;

    [[nodiscard]] bool canStart() const;
    void start();

    [[nodiscard]] QString serverUrl() const;
    void setServerUrl(const QString &serverUrl);

Q_SIGNALS:
    void fileDownloaded(const QByteArray &data);

private:
    LIBRUQOLACORE_NO_EXPORT void slotSslErrors(QNetworkReply *reply, const QList<QSslError> &error);
    LIBRUQOLACORE_NO_EXPORT void slotFileDownloaded(QNetworkReply *reply);
    QString mServerUrl;
    QNetworkAccessManager *const mNetworkAccessManager;
};
