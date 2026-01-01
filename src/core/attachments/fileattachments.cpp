/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "fileattachments.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(FileAttachments, Ruqola_FileAttachments)

#include "ruqola_debug.h"
#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
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
    if (index < 0 || index >= mFileAttachments.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return {};
    }
    return mFileAttachments.at(index);
}

void FileAttachments::parseMoreFileAttachments(const QJsonObject &fileAttachmentsObj)
{
    const int filesCount = fileAttachmentsObj["count"_L1].toInt();
    mOffset = fileAttachmentsObj["offset"_L1].toInt();
    mTotal = fileAttachmentsObj["total"_L1].toInt();
    parseFiles(fileAttachmentsObj);
    mFilesCount += filesCount;
}

void FileAttachments::parseFiles(const QJsonObject &fileAttachmentsObj)
{
    const QJsonArray fileAttachmentsArray = fileAttachmentsObj["files"_L1].toArray();
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
    mFilesCount = fileAttachmentsObj["count"_L1].toInt();
    mOffset = fileAttachmentsObj["offset"_L1].toInt();
    mTotal = fileAttachmentsObj["total"_L1].toInt();
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

const QList<File> &FileAttachments::fileAttachments() const
{
    return mFileAttachments;
}

void FileAttachments::setFileAttachments(const QList<File> &fileAttachments)
{
    mFileAttachments = fileAttachments;
}

void FileAttachments::addFileAttachments(const QList<File> &fileAttachments)
{
    for (const auto &att : fileAttachments) {
        if (!mFileAttachments.contains(att)) {
            mFileAttachments.append(att);
        }
    }
}

QDebug operator<<(QDebug d, const FileAttachments &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "fileAttachmentsCount" << t.filesCount() << "\n";
    for (int i = 0, total = t.fileAttachments().count(); i < total; ++i) {
        d.space() << t.fileAttachments().at(i) << "\n";
    }
    return d;
}
