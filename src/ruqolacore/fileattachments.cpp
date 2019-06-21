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

#include "ruqola_debug.h"
#include "fileattachments.h"
#include <QJsonArray>
#include <QJsonObject>

FileAttachments::FileAttachments()
{

}

bool FileAttachments::isEmpty() const
{
    return mFileAttachments.isEmpty();
}

void FileAttachments::clear()
{
    mFileAttachments.clear();    
}

int FileAttachments::count() const
{
    return mFileAttachments.count();
}

File FileAttachments::at(int index) const
{
    return mFileAttachments.at(index);
}

void FileAttachments::parseMoreFileAttachments(const QJsonObject &fileAttachmentsObj)
{
    const int filesCount = fileAttachmentsObj[QStringLiteral("count")].toInt();
    mOffset = fileAttachmentsObj[QStringLiteral("offset")].toInt();
    mTotal = fileAttachmentsObj[QStringLiteral("total")].toInt();
    parseFiles(fileAttachmentsObj);
    mFilesCount += filesCount;
}

void FileAttachments::parseFiles(const QJsonObject &fileAttachmentsObj)
{
    const QJsonArray fileAttachmentsArray = fileAttachmentsObj[QStringLiteral("files")].toArray();
    mFileAttachments.reserve(mFileAttachments.count() + fileAttachmentsArray.count());
    for (const QJsonValue &current : fileAttachmentsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject fileAttachmentObject = current.toObject();
            File m;
            m.parseFile(fileAttachmentObject, true);
            mFileAttachments.append(m);
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing file attachment" << current;
        }
    }
}

void FileAttachments::parseFileAttachments(const QJsonObject &fileAttachmentsObj)
{
    mFilesCount = fileAttachmentsObj[QStringLiteral("count")].toInt();
    mOffset = fileAttachmentsObj[QStringLiteral("offset")].toInt();
    mTotal = fileAttachmentsObj[QStringLiteral("total")].toInt();
    mFileAttachments.clear();
    parseFiles(fileAttachmentsObj);
}

int FileAttachments::filesCount() const
{
    return mFilesCount;
}

void FileAttachments::setFilesCount(int filesCount)
{
    mFilesCount = filesCount;
}

int FileAttachments::offset() const
{
    return mOffset;
}

void FileAttachments::setOffset(int offset)
{
    mOffset = offset;
}

int FileAttachments::total() const
{
    return mTotal;
}

void FileAttachments::setTotal(int total)
{
    mTotal = total;
}

QVector<File> FileAttachments::fileAttachments() const
{
    return mFileAttachments;
}

void FileAttachments::setFileAttachments(const QVector<File> &fileAttachments)
{
    mFileAttachments = fileAttachments;
}

QDebug operator <<(QDebug d, const FileAttachments &t)
{
    d << "total " << t.total();
    d << "offset " << t.offset();
    d << "fileAttachmentsCount " << t.filesCount();
    for (int i = 0, total = t.fileAttachments().count(); i < total; ++i) {
        d << t.fileAttachments().at(i);
    }
    return d;
}
