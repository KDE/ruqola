/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QJsonObject>
class LIBRUQOLACORE_EXPORT AppsCategoryInfo
{
public:
    AppsCategoryInfo();
    ~AppsCategoryInfo();

    void parseAppsCategoryInfo(const QJsonObject &replyObject);
    [[nodiscard]] QString title() const;
    void setTitle(const QString &newTitle);

    [[nodiscard]] bool hidden() const;
    void setHidden(bool newHidden);

    [[nodiscard]] QByteArray identifier() const;
    void setIdentifier(const QByteArray &newIdentifier);

    [[nodiscard]] bool isValid() const;

private:
    QByteArray mIdentifier;
    QString mTitle;
    // TODO description/createdDate/modifiedDate ?
    bool mHidden = false;
};

QT_DECL_METATYPE_EXTERN_TAGGED(AppsCategoryInfo, Ruqola_AppsCategoryInfo, LIBRUQOLACORE_EXPORT)
Q_DECLARE_TYPEINFO(AppsCategoryInfo, Q_RELOCATABLE_TYPE);
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AppsCategoryInfo &t);
