/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "file.h"
#include "libruqolacore_export.h"
#include "paginatedinfolist.h"
class QDebug;

namespace FileAttachmentsUtils
{
// File::parseFile() also wants to know the object comes from the REST API, while the container
// calls its element parser with the element and its json object only. This has to stay in the
// header: it is the base class's template argument below.
inline void parseRestApiFile(File &file, const QJsonObject &obj)
{
    file.parseFile(obj, true);
}
}

class LIBRUQOLACORE_EXPORT FileAttachments : public PaginatedInfoList<File, &FileAttachmentsUtils::parseRestApiFile>
{
public:
    void parseFileAttachments(const QJsonObject &obj);
    void parseMoreFileAttachments(const QJsonObject &obj);

    void addFileAttachments(const QList<File> &fileAttachments);
};

QT_DECL_METATYPE_EXTERN_TAGGED(FileAttachments, Ruqola_FileAttachments, LIBRUQOLACORE_EXPORT)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const FileAttachments &t);
