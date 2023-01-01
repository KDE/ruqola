/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "fileattachments.h"
#include "ruqola_debug.h"
#include <QJsonArray>
#include <QJsonObject>

FileAttachments::FileAttachments() = default;

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
    if (index < 0 || index > mFileAttachments.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return {};
    }
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
            mFileAttachments.append(std::move(m));
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

QDebug operator<<(QDebug d, const FileAttachments &t)
{
    d << "total " << t.total();
    d << "offset " << t.offset();
    d << "fileAttachmentsCount " << t.filesCount() << "\n";
    for (int i = 0, total = t.fileAttachments().count(); i < total; ++i) {
        d << t.fileAttachments().at(i) << "\n";
    }
    return d;
}
