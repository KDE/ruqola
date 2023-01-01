/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

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

    Q_REQUIRED_RESULT bool isEmpty() const;
    void clear();
    Q_REQUIRED_RESULT int count() const;
    Q_REQUIRED_RESULT File at(int index) const;

    Q_REQUIRED_RESULT int filesCount() const;
    void setFilesCount(int filesCount);

    Q_REQUIRED_RESULT int offset() const;
    void setOffset(int offset);

    Q_REQUIRED_RESULT int total() const;
    void setTotal(int total);

    Q_REQUIRED_RESULT QVector<File> fileAttachments() const;
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
