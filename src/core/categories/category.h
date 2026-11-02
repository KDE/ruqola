/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqola_private_export.h"
#include <QString>
class QDebug;
class LIBRUQOLACORE_TESTS_EXPORT Category
{
public:
    Category();

    [[nodiscard]] QByteArray identifier() const;
    void setIdentifier(const QByteArray &newIdentifier);

    [[nodiscard]] QString name() const;
    void setName(const QString &newName);

    [[nodiscard]] bool defaultCategory() const;
    void setDefaultCategory(bool newDefaultCategory);

    [[nodiscard]] bool showUnreads() const;
    void setShowUnreads(bool newShowUnreads);

    [[nodiscard]] bool operator==(const Category &other) const = default;

private:
    QByteArray mIdentifier;
    QString mName;
    bool mDefaultCategory = false;
    bool mShowUnreads = false;
};
QT_DECL_METATYPE_EXTERN_TAGGED(Category, Ruqola_Category, LIBRUQOLACORE_EXPORT)
Q_DECLARE_TYPEINFO(Category, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Category &t);
