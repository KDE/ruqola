/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "file.h"
#include "libruqola_private_export.h"
#include <QDebug>
#include <QVector>

class LIBRUQOLACORE_TESTS_EXPORT FileAttachments
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

    [[nodiscard]] QVector<File> fileAttachments() const;
    void setFileAttachments(const QVector<File> &fileAttachments);

    void parseFileAttachments(const QJsonObject &fileAttachmentsObj);
    void parseMoreFileAttachments(const QJsonObject &fileAttachmentsObj);

private:
    void parseFiles(const QJsonObject &fileAttachmentsObj);
    QVector<File> mFileAttachments;
    int mFilesCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};

Q_DECLARE_METATYPE(FileAttachments)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const FileAttachments &t);
