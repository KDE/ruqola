/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>

#include <QString>
class LIBRUQOLACORE_EXPORT AttachmentImageInfo
{
public:
    AttachmentImageInfo();
    ~AttachmentImageInfo();

    [[nodiscard]] bool isValid() const;

    void parseAttachmentImageInfo(const QJsonObject &object);

private:
    QByteArray mRoomId;
    QString mName;
    QString mFormat;
    QString mPath;
};
Q_DECLARE_METATYPE(AttachmentImageInfo)
Q_DECLARE_TYPEINFO(AttachmentImageInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AttachmentImageInfo &t);
