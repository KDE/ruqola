/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "attachmentimageinfo.h"
#include "libruqolacore_export.h"
#include <QDebug>
#include <QList>

class LIBRUQOLACORE_EXPORT AttachmentImageInfos
{
public:
    AttachmentImageInfos();
    ~AttachmentImageInfos();

    [[nodiscard]] int offset() const;
    void setOffset(int newOffset);

    [[nodiscard]] int total() const;
    void setTotal(int newTotal);

    [[nodiscard]] int attachmentImagesCount() const;
    void setAttachmentImagesCount(int newAttachmentImagesCount);

    [[nodiscard]] QList<AttachmentImageInfo> attachmentImageInfoList() const;
    void setAttachmentImageInfoList(const QList<AttachmentImageInfo> &newAttachmentImageInfoList);

private:
    int mAttachmentImagesCount = 0;
    int mOffset = 0;
    int mTotal = 0;
    QList<AttachmentImageInfo> mAttachmentImageInfoList;
};

Q_DECLARE_METATYPE(AttachmentImageInfos)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AttachmentImageInfos &t);
