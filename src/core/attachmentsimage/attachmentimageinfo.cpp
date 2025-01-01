/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "attachmentimageinfo.h"

#include <QJsonObject>

AttachmentImageInfo::AttachmentImageInfo() = default;

AttachmentImageInfo::~AttachmentImageInfo() = default;

void AttachmentImageInfo::parseAttachmentImageInfo(const QJsonObject &object)
{
}

bool AttachmentImageInfo::isValid() const
{
    // TODO
    return false;
}

QDebug operator<<(QDebug d, const AttachmentImageInfo &t)
{
    // TODO
    return d;
}
