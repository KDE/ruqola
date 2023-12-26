/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "file.h"
#include "utils.h"
#include <QDateTime>
#include <QJsonObject>

File::File() = default;

void File::parseFile(const QJsonObject &object, bool restApi)
{
    const QJsonObject fields = restApi ? object : object.value(QLatin1String("fields")).toObject();
    setUserId(fields.value(QLatin1String("userId")).toString());

    setDescription(fields.value(QLatin1String("description")).toString());
    setFileName(fields.value(QLatin1String("name")).toString());
    setMimeType(fields.value(QLatin1String("type")).toString());
    setUrl(fields.value(QLatin1String("url")).toString());
    setRid(fields.value(QLatin1String("rid")).toString());
    setComplete(fields.value(QLatin1String("complete")).toBool());
    setTypeGroup(fields.value(QLatin1String("typeGroup")).toString());
    if (restApi) {
        setUploadedAt(Utils::parseIsoDate(QStringLiteral("uploadedAt"), fields));
    } else {
        setUploadedAt(Utils::parseDate(QStringLiteral("uploadedAt"), fields));
    }

    const QJsonObject user = object.value(QLatin1String("user")).toObject();
    setUserName(user.value(QLatin1String("username")).toString());

    setFileId(restApi ? object.value(QLatin1String("_id")).toString() : object.value(QLatin1String("id")).toString());
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
        && (mimeType() == other.mimeType()) && (uploadedAt() == other.uploadedAt()) && (fileId() == other.fileId()) && (rid() == other.rid())
        && (userName() == other.userName()) && (complete() == other.complete()) && (typeGroup() == other.typeGroup());
}

QString File::userId() const
{
    return mUserId;
}

void File::setUserId(const QString &userId)
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

QString File::fileId() const
{
    return mFileId;
}

void File::setFileId(const QString &fileId)
{
    mFileId = fileId;
}

QString File::rid() const
{
    return mRid;
}

void File::setRid(const QString &rid)
{
    mRid = rid;
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

QDebug operator<<(QDebug d, const File &t)
{
    d.space() << "Name:" << t.fileName();
    d.space() << "Description:" << t.description();
    d.space() << "Url:" << t.url();
    d.space() << "UserId:" << t.userId();
    d.space() << "Mimetype:" << t.mimeType();
    d.space() << "Uploaded time:" << t.uploadedAt();
    d.space() << "File Id:" << t.fileId();
    d.space() << "Rid:" << t.rid();
    d.space() << "Username:" << t.userName();
    d.space() << "Complete:" << t.complete();
    d.space() << "TypeGroup:" << t.typeGroup();
    return d;
}

#include "moc_file.cpp"
