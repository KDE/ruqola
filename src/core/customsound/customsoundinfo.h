/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QMetaType>
#include <QString>
class QJsonObject;
class LIBRUQOLACORE_EXPORT CustomSoundInfo
{
public:
    CustomSoundInfo();
    ~CustomSoundInfo() = default;

    [[nodiscard]] const QByteArray &identifier() const;
    void setIdentifier(const QByteArray &newIdentifier);

    [[nodiscard]] const QString &name() const;
    void setName(const QString &newName);

    void parseCustomSoundInfo(const QJsonObject &obj);

    [[nodiscard]] const QString &extension() const;
    void setExtension(const QString &newExtension);

    [[nodiscard]] bool isValid() const;

    [[nodiscard]] bool defaultSound() const;
    void setDefaultSound(bool newDefaultSound);

    [[nodiscard]] QString generateUrl() const;

private:
    QByteArray mIdentifier;
    QString mName;
    QString mExtension;
    bool mDefaultSound = false;
};

QT_DECL_METATYPE_EXTERN_TAGGED(CustomSoundInfo, Ruqola_CustomSoundInfo, LIBRUQOLACORE_EXPORT)
Q_DECLARE_TYPEINFO(CustomSoundInfo, Q_RELOCATABLE_TYPE);
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const CustomSoundInfo &t);
