/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>

class LIBRUQOLACORE_EXPORT OauthInfo
{
public:
    OauthInfo();
    ~OauthInfo() = default;
    Q_REQUIRED_RESULT bool operator==(const OauthInfo &other) const;
    void parseOauthInfo(const QJsonObject &replyObject);

    Q_REQUIRED_RESULT const QString &identifier() const;
    void setIdentifier(const QString &newIdentifier);

    Q_REQUIRED_RESULT bool active() const;
    void setActive(bool newActive);

    Q_REQUIRED_RESULT const QString &name() const;
    void setName(const QString &newName);

    Q_REQUIRED_RESULT const QString &clientId() const;
    void setClientId(const QString &newClientId);

    Q_REQUIRED_RESULT const QString &clientSecret() const;
    void setClientSecret(const QString &newClientSecret);

    Q_REQUIRED_RESULT const QString &redirectUri() const;
    void setRedirectUri(const QString &newRedirectUri);

private:
    QString mIdentifier;
    QString mName;
    QString mClientId;
    QString mClientSecret;
    QString mRedirectUri;
    bool mActive = false;
};

Q_DECLARE_METATYPE(OauthInfo)
Q_DECLARE_TYPEINFO(OauthInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const OauthInfo &t);
