/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentfield.h"

MessageAttachmentField::MessageAttachmentField() = default;

MessageAttachmentField::~MessageAttachmentField() = default;

bool MessageAttachmentField::operator==(const MessageAttachmentField &other) const
{
    return mTitle == other.title() && mValue == other.value();
}

QString MessageAttachmentField::title() const
{
    return mTitle;
}

void MessageAttachmentField::setTitle(const QString &title)
{
    mTitle = title;
}

QString MessageAttachmentField::value() const
{
    return mValue;
}

void MessageAttachmentField::setValue(const QString &value)
{
    mValue = value;
}

QJsonObject MessageAttachmentField::serialize(const MessageAttachmentField &message)
{
    QJsonObject obj;
    obj[QStringLiteral("title")] = message.title();
    obj[QStringLiteral("value")] = message.value();
    return obj;
}

MessageAttachmentField MessageAttachmentField::fromJson(const QJsonObject &o)
{
    MessageAttachmentField att;
    att.setValue(o.value(QLatin1String("value")).toString());
    att.setTitle(o.value(QLatin1String("title")).toString());
    return att;
}

QDebug operator<<(QDebug d, const MessageAttachmentField &t)
{
    d << " title " << t.title();
    d << " value " << t.value();
    return d;
}
