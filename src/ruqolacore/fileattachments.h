/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef FILEATTACHMENTS_H
#define FILEATTACHMENTS_H

#include "file.h"
#include "libruqola_private_export.h"
#include "thread.h"
#include <QVector>
#include <QDebug>

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
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const FileAttachments &t);

#endif // FILEATTACHMENTS_H
