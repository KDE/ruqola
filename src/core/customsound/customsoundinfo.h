/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>

class LIBRUQOLACORE_EXPORT CustomSoundInfo
{
public:
    CustomSoundInfo();
    ~CustomSoundInfo() = default;

    [[nodiscard]] const QString &identifier() const;
    void setIdentifier(const QString &newIdentifier);

    [[nodiscard]] const QString &name() const;
    void setName(const QString &newName);

    void parseCustomSoundInfo(const QJsonObject &obj);

    [[nodiscard]] const QString &extension() const;
    void setExtension(const QString &newExtension);

    [[nodiscard]] bool isValid() const;

private:
    QString mIdentifier;
    QString mName;
    QString mExtension;
};

Q_DECLARE_METATYPE(CustomSoundInfo)
Q_DECLARE_TYPEINFO(CustomSoundInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const CustomSoundInfo &t);
