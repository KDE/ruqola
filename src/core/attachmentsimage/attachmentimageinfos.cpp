/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "attachmentimageinfos.h"

AttachmentImageInfos::AttachmentImageInfos() = default;

AttachmentImageInfos::~AttachmentImageInfos() = default;

QDebug operator<<(QDebug d, const AttachmentImageInfos &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "commandsCount" << t.attachmentImagesCount() << "\n";
    for (int i = 0, total = t.attachmentImageInfoList().count(); i < total; ++i) {
        d.space() << t.attachmentImageInfoList().at(i) << "\n";
    }
    return d;
}

int AttachmentImageInfos::offset() const
{
    return mOffset;
}

void AttachmentImageInfos::setOffset(int newOffset)
{
    mOffset = newOffset;
}

int AttachmentImageInfos::total() const
{
    return mTotal;
}

void AttachmentImageInfos::setTotal(int newTotal)
{
    mTotal = newTotal;
}

int AttachmentImageInfos::attachmentImagesCount() const
{
    return mAttachmentImagesCount;
}

void AttachmentImageInfos::setAttachmentImagesCount(int newAttachmentImagesCount)
{
    mAttachmentImagesCount = newAttachmentImagesCount;
}

QList<AttachmentImageInfo> AttachmentImageInfos::attachmentImageInfoList() const
{
    return mAttachmentImageInfoList;
}

void AttachmentImageInfos::setAttachmentImageInfoList(const QList<AttachmentImageInfo> &newAttachmentImageInfoList)
{
    mAttachmentImageInfoList = newAttachmentImageInfoList;
}
