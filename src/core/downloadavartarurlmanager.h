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

#ifndef DOWNLOADAVARTARURLMANAGER_H
#define DOWNLOADAVARTARURLMANAGER_H

#include <QObject>
#include <QMap>
#include <QSslError>
#include "libruqolacore_export.h"
class QNetworkAccessManager;
class QNetworkReply;
class LIBRUQOLACORE_EXPORT DownloadAvartarUrlManager : public QObject
{
    Q_OBJECT
public:
    explicit DownloadAvartarUrlManager(QObject *parent = nullptr);
    ~DownloadAvartarUrlManager();

    Q_REQUIRED_RESULT bool canStart() const;
    void start();

    Q_REQUIRED_RESULT QString avatarUrl() const;
    void setAvatarUrl(const QString &avatarUrl);
Q_SIGNALS:
    void avatarDownloaded(const QByteArray &data, const QString &avatarUrl);
private:
    void slotFileDownloaded(QNetworkReply *reply);
    void slotSslErrors(QNetworkReply *reply, const QList<QSslError> &error);
    QString mAvatarUrl;
    QNetworkAccessManager *const mNetworkAccessManager;
};

#endif // DOWNLOADAVARTARURLMANAGER_H
