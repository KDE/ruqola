/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QDebug>
#include <QString>

class LIBRUQOLACORE_EXPORT PreviewCommand
{
public:
    PreviewCommand();
    ~PreviewCommand();
    [[nodiscard]] QString id() const;
    void setId(const QString &newId);

    [[nodiscard]] QString value() const;
    void setValue(const QString &newValue);

    [[nodiscard]] QString type() const;
    void setType(const QString &newType);

private:
    QString mId;
    QString mValue;
    QString mType;
};

Q_DECLARE_METATYPE(PreviewCommand)
Q_DECLARE_TYPEINFO(PreviewCommand, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const PreviewCommand &t);
