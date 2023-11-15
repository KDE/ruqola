/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>

class LIBRUQOLACORE_EXPORT MessageAttachmentField
{
public:
    MessageAttachmentField();
    ~MessageAttachmentField();
    [[nodiscard]] bool operator==(const MessageAttachmentField &other) const;
    [[nodiscard]] QString title() const;
    void setTitle(const QString &title);

    [[nodiscard]] QString value() const;
    void setValue(const QString &value);

    static QJsonObject serialize(const MessageAttachmentField &message);
    static MessageAttachmentField deserialize(const QJsonObject &o);

private:
    QString mTitle;
    QString mValue;
};
Q_DECLARE_TYPEINFO(MessageAttachmentField, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const MessageAttachmentField &t);
