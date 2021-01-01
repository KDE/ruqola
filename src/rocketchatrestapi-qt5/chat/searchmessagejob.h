/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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

#ifndef SEARCHMESSAGEJOB_H
#define SEARCHMESSAGEJOB_H

#include "restapiabstractjob.h"
#include "librestapi_private_export.h"

#include <QNetworkRequest>
namespace RocketChatRestApi {
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT SearchMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit SearchMessageJob(QObject *parent = nullptr);
    ~SearchMessageJob() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

    Q_REQUIRED_RESULT QString searchText() const;
    void setSearchText(const QString &searchText);

    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT int count() const;
    void setCount(int count);

    Q_REQUIRED_RESULT bool useRegularExpression() const;
    void setUseRegularExpression(bool useRegularExpression);

Q_SIGNALS:
    void searchMessageDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(SearchMessageJob)
    void slotSearchMessageFinished();
    Q_REQUIRED_RESULT QString convertSearchText() const;
    QString mRoomId;
    QString mSearchText;
    bool mUseRegularExpression = false;
    int mCount = -1;
};
}
#endif // SEARCHMESSAGEJOB_H
