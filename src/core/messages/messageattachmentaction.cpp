/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentaction.h"
#include <QDebug>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
MessageAttachmentAction::MessageAttachmentAction() = default;

MessageAttachmentAction::~MessageAttachmentAction() = default;

bool MessageAttachmentAction::isValid() const
{
    return !mType.isEmpty();
}

void MessageAttachmentAction::parse(const QJsonObject &obj)
{
    mMsg = obj["msg"_L1].toString();
    mText = obj["text"_L1].toString();
    mType = obj["type"_L1].toString();
}

QString MessageAttachmentAction::text() const
{
    return mText;
}

void MessageAttachmentAction::setText(const QString &newText)
{
    mText = newText;
}

bool MessageAttachmentAction::operator==(const MessageAttachmentAction &other) const
{
    return mText == other.text() && mMsg == other.msg() && mType == other.type();
}

QString MessageAttachmentAction::msg() const
{
    return mMsg;
}

void MessageAttachmentAction::setMsg(const QString &newMsg)
{
    mMsg = newMsg;
}

QString MessageAttachmentAction::type() const
{
    return mType;
}

void MessageAttachmentAction::setType(const QString &newType)
{
    mType = newType;
}

QDebug operator<<(QDebug d, const MessageAttachmentAction &t)
{
    d.space() << "text:" << t.text();
    d.space() << "msg:" << t.msg();
    d.space() << "type:" << t.type();
    return d;
}
