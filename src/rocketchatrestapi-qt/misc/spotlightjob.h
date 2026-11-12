/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
class QDebug;
class QUrlQuery;

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT SpotlightJob final : public RestApiAbstractJob
{
    Q_OBJECT
public:
    enum class SpotlightJobType : uint8_t {
        Unknown = 0,
        Users = 1,
        Rooms = 2,
        Mentions = 4,
    };
    Q_DECLARE_FLAGS(SpotlightJobTypes, SpotlightJobType)
    Q_FLAG(SpotlightJobTypes)

    struct LIBROCKETCHATRESTAPI_QT_EXPORT SpotlightJobInfo {
        QString pattern;
        QStringList excludeUsernames;
        SpotlightJobTypes types = SpotlightJobType::Unknown;
        QByteArray roomId;
        void addQueryItem(QUrlQuery &queryUrl) const;
        [[nodiscard]] bool isValid() const;
        [[nodiscard]] QString convertSpotlightTypeToString() const;
    };

    explicit SpotlightJob(QObject *parent = nullptr);
    ~SpotlightJob() override;

    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] SpotlightJobInfo spotlightInfo() const;
    void setSpotlightInfo(const SpotlightJobInfo &newSpotlightInfo);

Q_SIGNALS:
    void spotlightDone(const QJsonObject &obj);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    SpotlightJobInfo mSpotlightInfo;
};
}
Q_DECLARE_OPERATORS_FOR_FLAGS(RocketChatRestApi::SpotlightJob::SpotlightJobTypes)
Q_DECLARE_TYPEINFO(RocketChatRestApi::SpotlightJob::SpotlightJobInfo, Q_RELOCATABLE_TYPE);
LIBROCKETCHATRESTAPI_QT_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::SpotlightJob::SpotlightJobInfo &t);
