/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolacore_export.h"
#include "messages/messageattachmentaction.h"
#include <QMetaType>
class QDebug;
class QJsonObject;
class LIBRUQOLACORE_EXPORT MessageAttachmentActions
{
    Q_GADGET
public:
    enum class AlignmentButton : uint8_t {
        Unknown = 0,
        Horizontal,
        Vertical,
    };
    Q_ENUM(AlignmentButton)
    MessageAttachmentActions();
    ~MessageAttachmentActions();

    void parse(const QJsonObject &obj);
    [[nodiscard]] AlignmentButton alignment() const;
    void setAlignment(const AlignmentButton &newAlignment);

    [[nodiscard]] bool operator==(const MessageAttachmentActions &other) const;

    [[nodiscard]] QList<MessageAttachmentAction> actions() const;
    void setActions(const QList<MessageAttachmentAction> &newActions);

    [[nodiscard]] AlignmentButton convertStringToAlignmentButton(const QString &str) const;

    [[nodiscard]] bool isValid() const;

private:
    AlignmentButton mAlignment = AlignmentButton::Unknown;
    QList<MessageAttachmentAction> mActions;
};
Q_DECLARE_TYPEINFO(MessageAttachmentActions, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const MessageAttachmentActions &t);
