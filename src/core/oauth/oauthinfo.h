/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDateTime>

class LIBRUQOLACORE_EXPORT OauthInfo
{
public:
    OauthInfo();
    ~OauthInfo() = default;
    [[nodiscard]] bool operator==(const OauthInfo &other) const;
    void parseOauthInfo(const QJsonObject &replyObject, bool restapi = true);

    [[nodiscard]] const QString &identifier() const;
    void setIdentifier(const QString &newIdentifier);

    [[nodiscard]] bool active() const;
    void setActive(bool newActive);

    [[nodiscard]] const QString &name() const;
    void setName(const QString &newName);

    [[nodiscard]] const QString &clientId() const;
    void setClientId(const QString &newClientId);

    [[nodiscard]] const QString &clientSecret() const;
    void setClientSecret(const QString &newClientSecret);

    [[nodiscard]] const QString &redirectUri() const;
    void setRedirectUri(const QString &newRedirectUri);

    [[nodiscard]] const QString &createdBy() const;
    void setCreatedBy(const QString &newCreatedBy);

    [[nodiscard]] const QDateTime &createdDateTime() const;
    void setCreatedDateTime(const QDateTime &newCreatedDateTime);

private:
    QString mIdentifier;
    QString mName;
    QString mClientId;
    QString mClientSecret;
    QString mRedirectUri;
    QString mCreatedBy;
    QDateTime mCreatedDateTime;
    bool mActive = false;
};

QT_DECL_METATYPE_EXTERN_TAGGED(OauthInfo, Ruqola_OauthInfo, LIBRUQOLACORE_EXPORT)
Q_DECLARE_TYPEINFO(OauthInfo, Q_RELOCATABLE_TYPE);
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const OauthInfo &t);
