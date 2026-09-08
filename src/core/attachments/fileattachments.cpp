/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "fileattachments.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(FileAttachments, Ruqola_FileAttachments)

using namespace Qt::Literals::StringLiterals;

void FileAttachments::parseFileAttachments(const QJsonObject &obj)
{
    parseInfos(obj, "files"_L1);
}

void FileAttachments::parseMoreFileAttachments(const QJsonObject &obj)
{
    parseMoreInfos(obj, "files"_L1);
}

void FileAttachments::addFileAttachments(const QList<File> &fileAttachments)
{
    for (const File &attachment : fileAttachments) {
        if (!mList.contains(attachment)) {
            mList.append(attachment);
        }
    }
}

QDebug operator<<(QDebug d, const FileAttachments &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "fileAttachmentsCount" << t.loadedCount() << "\n";
    for (const File &file : t.list()) {
        d.space() << file << "\n";
    }
    return d;
}
