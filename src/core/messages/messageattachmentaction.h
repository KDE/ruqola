/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolacore_export.h"
#include <QMetaType>
class QJsonObject;
class QDebug;
class LIBRUQOLACORE_EXPORT MessageAttachmentAction
{
public:
    MessageAttachmentAction();
    ~MessageAttachmentAction();

    void parse(const QJsonObject &obj);
    [[nodiscard]] QString text() const;
    void setText(const QString &newText);

    [[nodiscard]] bool operator==(const MessageAttachmentAction &other) const;

    [[nodiscard]] QString msg() const;
    void setMsg(const QString &newMsg);

    [[nodiscard]] QString type() const;
    void setType(const QString &newType);

private:
    QString mText;
    QString mMsg;
    QString mType;
};

Q_DECLARE_TYPEINFO(MessageAttachmentAction, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const MessageAttachmentAction &t);
