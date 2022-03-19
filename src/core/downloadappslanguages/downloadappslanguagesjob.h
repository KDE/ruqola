/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

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

    Q_REQUIRED_RESULT bool canStart() const;
    void start();

    Q_REQUIRED_RESULT QString serverUrl() const;
    void setServerUrl(const QString &serverUrl);

Q_SIGNALS:
    void fileDownloaded(const QByteArray &data);

private:
    void slotSslErrors(QNetworkReply *reply, const QList<QSslError> &error);
    void slotFileDownloaded(QNetworkReply *reply);
    QString mServerUrl;
    QNetworkAccessManager *const mNetworkAccessManager;
};
