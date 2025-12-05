/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "file.h"
#include "utils.h"
#include <QDateTime>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
File::File() = default;

void File::parseFile(const QJsonObject &object, bool restApi)
{
    // qDebug() << " object " << object;
    const QJsonObject fields = restApi ? object : object.value("fields"_L1).toObject();
    setUserId(fields.value("userId"_L1).toString().toLatin1());

    setDescription(fields.value("description"_L1).toString());
    setFileName(fields.value("name"_L1).toString());
    setMimeType(fields.value("type"_L1).toString());
    setUrl(fields.value("url"_L1).toString());
    setComplete(fields.value("complete"_L1).toBool());
    setTypeGroup(fields.value("typeGroup"_L1).toString());
    setPath(fields.value("path"_L1).toString());
    setRoomId(fields.value("rid"_L1).toString().toLatin1());
    if (restApi) {
        setUploadedAt(Utils::parseIsoDate(u"uploadedAt"_s, fields));
    } else {
        setUploadedAt(Utils::parseDate(u"uploadedAt"_s, fields));
    }

    const QJsonObject user = object.value("user"_L1).toObject();
    setUserName(user.value("username"_L1).toString());

    setFileId(restApi ? object.value("_id"_L1).toString().toLatin1() : object.value("id"_L1).toString().toLatin1());
}

QString File::fileName() const
{
    return mFileName;
}

void File::setFileName(const QString &name)
{
    mFileName = name;
}

QString File::description() const
{
    return mDescription;
}

void File::setDescription(const QString &description)
{
    mDescription = description;
}

bool File::operator==(const File &other) const
{
    return (description() == other.description()) && (fileName() == other.fileName()) && (url() == other.url()) && (userId() == other.userId())
        && (mimeType() == other.mimeType()) && (uploadedAt() == other.uploadedAt()) && (fileId() == other.fileId()) && (userName() == other.userName())
        && (complete() == other.complete()) && (typeGroup() == other.typeGroup()) && (path() == other.path()) && (roomId() == other.roomId());
}

QByteArray File::userId() const
{
    return mUserId;
}

void File::setUserId(const QByteArray &userId)
{
    mUserId = userId;
}

QString File::url() const
{
    return mUrl;
}

void File::setUrl(const QString &url)
{
    mUrl = url;
}

QString File::mimeType() const
{
    return mMimeType;
}

void File::setMimeType(const QString &mimeType)
{
    mMimeType = mimeType;
}

qint64 File::uploadedAt() const
{
    return mUploadedAt;
}

void File::setUploadedAt(qint64 uploadedAt)
{
    mUploadedAt = uploadedAt;
    QLocale l;
    mUploadedDateTimeStr = l.toString(QDateTime::fromMSecsSinceEpoch(mUploadedAt), QLocale::LongFormat);
}

QByteArray File::fileId() const
{
    return mFileId;
}

void File::setFileId(const QByteArray &fileId)
{
    mFileId = fileId;
}

QString File::userName() const
{
    return mUserName;
}

void File::setUserName(const QString &userName)
{
    mUserName = userName;
}

QString File::uploadedDateTimeStr() const
{
    return mUploadedDateTimeStr;
}

bool File::complete() const
{
    return mComplete;
}

void File::setComplete(bool complete)
{
    mComplete = complete;
}

QString File::typeGroup() const
{
    return mTypeGroup;
}

void File::setTypeGroup(const QString &typeGroup)
{
    mTypeGroup = typeGroup;
}

QString File::path() const
{
    return mPath;
}

void File::setPath(const QString &newPath)
{
    mPath = newPath;
}

QByteArray File::roomId() const
{
    return mRoomId;
}

void File::setRoomId(const QByteArray &newRoomId)
{
    mRoomId = newRoomId;
}

QDebug operator<<(QDebug d, const File &t)
{
    d.space() << "Name:" << t.fileName();
    d.space() << "Description:" << t.description();
    d.space() << "Url:" << t.url();
    d.space() << "UserId:" << t.userId();
    d.space() << "Mimetype:" << t.mimeType();
    d.space() << "Uploaded time:" << t.uploadedAt();
    d.space() << "File Id:" << t.fileId();
    d.space() << "Username:" << t.userName();
    d.space() << "Complete:" << t.complete();
    d.space() << "TypeGroup:" << t.typeGroup();
    d.space() << "Path:" << t.path();
    d.space() << "RoomId:" << t.roomId();
    return d;
}

#include "moc_file.cpp"
