/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT RoomsSetCategoryJob final : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit RoomsSetCategoryJob(QObject *parent = nullptr);
    ~RoomsSetCategoryJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QByteArray categoryId() const;
    void setCategoryId(const QByteArray &newCategoryId);

    [[nodiscard]] QList<QByteArray> roomIds() const;
    void setRoomIds(const QList<QByteArray> &newRoomIds);

Q_SIGNALS:
    void roomsSetCategoryDone();

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QByteArray mCategoryId;
    QList<QByteArray> mRoomIds;
};
}
