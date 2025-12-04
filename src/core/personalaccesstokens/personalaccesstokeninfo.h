/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqola_private_export.h"
#include <QObject>
class LIBRUQOLACORE_TESTS_EXPORT PersonalAccessTokenInfo
{
public:
    PersonalAccessTokenInfo();
    [[nodiscard]] bool bypassTwoFactor() const;
    void setBypassTwoFactor(bool newBypassTwoFactor);

    [[nodiscard]] const QString &name() const;
    void setName(const QString &newName);

    [[nodiscard]] const QString &lastTokenPart() const;
    void setLastTokenPart(const QString &newLastTokenPart);

    [[nodiscard]] bool isValid() const;

    void parsePersonalAccessTokenInfo(const QJsonObject &replyObject);

    [[nodiscard]] qint64 createdAt() const;
    void setCreatedAt(qint64 newCreatedAt);

    const QString &createAtDisplayDateTime() const;
    [[nodiscard]] bool operator==(const PersonalAccessTokenInfo &other) const;

private:
    QString mLastTokenPart;
    QString mName;
    QString mCreateAtDisplayDateTime;
    qint64 mCreatedAt = -1;
    bool mBypassTwoFactor = false;
};

Q_DECLARE_METATYPE(PersonalAccessTokenInfo)
Q_DECLARE_TYPEINFO(PersonalAccessTokenInfo, Q_RELOCATABLE_TYPE);
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const PersonalAccessTokenInfo &t);
