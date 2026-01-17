/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
class QDebug;
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT SearchMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit SearchMessageJob(QObject *parent = nullptr);
    ~SearchMessageJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &roomId);

    [[nodiscard]] QString searchText() const;
    void setSearchText(const QString &searchText);

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] int count() const;
    void setCount(int count);

    [[nodiscard]] bool useRegularExpression() const;
    void setUseRegularExpression(bool useRegularExpression);

    [[nodiscard]] int offset() const;
    void setOffset(int newOffset);

Q_SIGNALS:
    void searchMessageDone(const QJsonObject &obj);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    [[nodiscard]] LIBROCKETCHATRESTAPI_QT_NO_EXPORT QString convertSearchText() const;
    QString mRoomId;
    QString mSearchText;
    bool mUseRegularExpression = false;
    int mOffset = -1;
    int mCount = -1;
};
}
LIBROCKETCHATRESTAPI_QT_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::SearchMessageJob &t);
