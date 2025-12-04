/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "file.h"
#include "libruqolacore_export.h"
#include <QList>
class QJsonObject;

class LIBRUQOLACORE_EXPORT FileAttachments
{
public:
    FileAttachments();

    [[nodiscard]] bool isEmpty() const;
    void clear();
    [[nodiscard]] int count() const;
    [[nodiscard]] File at(int index) const;

    [[nodiscard]] int filesCount() const;
    void setFilesCount(int filesCount);

    [[nodiscard]] int offset() const;
    void setOffset(int offset);

    [[nodiscard]] int total() const;
    void setTotal(int total);

    [[nodiscard]] const QList<File> &fileAttachments() const;
    void setFileAttachments(const QList<File> &fileAttachments);

    void parseFileAttachments(const QJsonObject &fileAttachmentsObj);
    void parseMoreFileAttachments(const QJsonObject &fileAttachmentsObj);

    void addFileAttachments(const QList<File> &fileAttachments);

private:
    LIBRUQOLACORE_NO_EXPORT void parseFiles(const QJsonObject &fileAttachmentsObj);
    QList<File> mFileAttachments;
    int mFilesCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};

QT_DECL_METATYPE_EXTERN_TAGGED(FileAttachments, Ruqola_FileAttachments, LIBRUQOLACORE_EXPORT)
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const FileAttachments &t);
