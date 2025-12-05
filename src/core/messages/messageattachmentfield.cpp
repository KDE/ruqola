/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentfield.h"

#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
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
    obj["title"_L1] = message.title();
    obj["value"_L1] = message.value();
    return obj;
}

MessageAttachmentField MessageAttachmentField::deserialize(const QJsonObject &o)
{
    MessageAttachmentField att;
    att.setValue(o.value("value"_L1).toString());
    att.setTitle(o.value("title"_L1).toString());
    return att;
}

QDebug operator<<(QDebug d, const MessageAttachmentField &t)
{
    d.space() << "title" << t.title();
    d.space() << "value" << t.value();
    return d;
}
