/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqola_private_export.h"
#include <QDebug>
#include <QJsonObject>
#include <QVector>

class LIBRUQOLACORE_TESTS_EXPORT PersonalAccessTokenInfo
{
public:
    PersonalAccessTokenInfo();
    Q_REQUIRED_RESULT bool bypassTwoFactor() const;
    void setBypassTwoFactor(bool newBypassTwoFactor);

    Q_REQUIRED_RESULT const QString &name() const;
    void setName(const QString &newName);

    Q_REQUIRED_RESULT const QString &lastTokenPart() const;
    void setLastTokenPart(const QString &newLastTokenPart);

    Q_REQUIRED_RESULT bool isValid() const;

    void parsePersonalAccessTokenInfo(const QJsonObject &replyObject);

    Q_REQUIRED_RESULT qint64 createdAt() const;
    void setCreatedAt(qint64 newCreatedAt);

private:
    qint64 mCreatedAt = -1;
    QString mLastTokenPart;
    QString mName;
    bool mBypassTwoFactor = false;
};

Q_DECLARE_METATYPE(PersonalAccessTokenInfo)
Q_DECLARE_TYPEINFO(PersonalAccessTokenInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const PersonalAccessTokenInfo &t);
