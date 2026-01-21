/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "messageattachmentactions.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
// packages/core-typings/src/IMessage/MessageAttachment/MessageAttachmentAction.ts
using namespace Qt::Literals::StringLiterals;
MessageAttachmentActions::MessageAttachmentActions() = default;

MessageAttachmentActions::~MessageAttachmentActions() = default;

void MessageAttachmentActions::parse(const QJsonObject &obj)
{
    mAlignment = convertStringToAlignmentButton(obj["button_alignment"_L1].toString());
    // TODO load actions
}

MessageAttachmentActions::AlignmentButton MessageAttachmentActions::alignment() const
{
    return mAlignment;
}

void MessageAttachmentActions::setAlignment(const AlignmentButton &newAlignment)
{
    mAlignment = newAlignment;
}

bool MessageAttachmentActions::operator==(const MessageAttachmentActions &other) const
{
    return mAlignment == other.alignment() && mActions == other.actions();
}

QList<MessageAttachmentAction> MessageAttachmentActions::actions() const
{
    return mActions;
}

void MessageAttachmentActions::setActions(const QList<MessageAttachmentAction> &newActions)
{
    mActions = newActions;
}

QDebug operator<<(QDebug d, const MessageAttachmentActions &t)
{
    d.space() << "alignment:" << t.alignment();
    d.space() << "actions:" << t.actions();
    return d;
}

MessageAttachmentActions::AlignmentButton MessageAttachmentActions::convertStringToAlignmentButton(const QString &str) const
{
    if (str == "horizontal"_L1) {
        return MessageAttachmentActions::AlignmentButton::Horizontal;
    } else if (str == "vertical"_L1) {
        return MessageAttachmentActions::AlignmentButton::Vertical;
    } else {
        return MessageAttachmentActions::AlignmentButton::Unknown;
    }
}
#include "moc_messageattachmentactions.cpp"
