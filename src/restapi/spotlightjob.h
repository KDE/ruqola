/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#ifndef SPOTLIGHTJOB_H
#define SPOTLIGHTJOB_H

#include "restapiabstractjob.h"

#include "librestapi_private_export.h"

#include <QNetworkRequest>
namespace RocketChatRestApi {
class LIBRESTAPI_TESTS_EXPORT SpotlightJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit SpotlightJob(QObject *parent = nullptr);
    ~SpotlightJob() override;

    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QString searchPattern() const;
    void setSearchPattern(const QString &searchPattern);

Q_SIGNALS:
    void spotlightDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(SpotlightJob)
    void slotSpotlightDone();
    QString mSearchPattern;
};
}
#endif // SPOTLIGHTJOB_H
