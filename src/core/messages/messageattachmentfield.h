/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>
class LIBRUQOLACORE_EXPORT MessageAttachmentField
{
public:
    MessageAttachmentField();
    ~MessageAttachmentField();
    Q_REQUIRED_RESULT bool operator==(const MessageAttachmentField &other) const;
    Q_REQUIRED_RESULT QString title() const;
    void setTitle(const QString &title);

    Q_REQUIRED_RESULT QString value() const;
    void setValue(const QString &value);

    static QJsonObject serialize(const MessageAttachmentField &message);
    static MessageAttachmentField fromJson(const QJsonObject &o);

private:
    QString mTitle;
    QString mValue;
};
Q_DECLARE_TYPEINFO(MessageAttachmentField, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const MessageAttachmentField &t);
