/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT SearchMessageJob : public RestApiAbstractJob
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
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    Q_REQUIRED_RESULT LIBROCKETCHATRESTAPI_QT5_NO_EXPORT QString convertSearchText() const;
    QString mRoomId;
    QString mSearchText;
    bool mUseRegularExpression = false;
    int mCount = -1;
};
}
