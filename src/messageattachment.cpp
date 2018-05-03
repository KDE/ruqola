/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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

#include "messageattachment.h"
#include <KLocalizedString>
#include <QJsonObject>

MessageAttachment::MessageAttachment()
{
}

QJsonObject MessageAttachment::serialize(const MessageAttachment &message)
{
    QJsonObject obj;
    obj[QStringLiteral("description")] = message.description();
    obj[QStringLiteral("title")] = message.title();
    obj[QStringLiteral("url")] = message.link();
    const QString authorname = message.authorName();
    if (!authorname.isEmpty()) {
        obj[QStringLiteral("authorname")] = authorname;
    }
    const QString mimeType = message.mimeType();
    if (!mimeType.isEmpty()) {
        obj[QStringLiteral("mimetype")] = mimeType;
    }
    if ((message.imageHeight() != -1) && (message.imageWidth() != -1)) {
        obj[QStringLiteral("image_height")] = message.imageHeight();
        obj[QStringLiteral("image_width")] = message.imageWidth();
    }
    const QString color = message.color();
    if (!color.isEmpty()) {
        obj[QStringLiteral("color")] = color;
    }
    return obj;
}

MessageAttachment MessageAttachment::fromJSon(const QJsonObject &o)
{
    MessageAttachment att;
    att.setDescription(o.value(QLatin1String("description")).toString());
    att.setTitle(o.value(QLatin1String("title")).toString());
    att.setLink(o.value(QLatin1String("url")).toString());
    att.setAuthorName(o.value(QLatin1String("authorname")).toString());
    att.setMimeType(o.value(QLatin1String("image_type")).toString());
    const QJsonValue valHeight = o.value(QLatin1String("image_height"));
    if (!valHeight.isUndefined()) {
        att.setImageHeight(valHeight.toInt());
    }
    const QJsonValue valWidth = o.value(QLatin1String("image_width"));
    if (!valWidth.isUndefined()) {
        att.setImageWidth(valWidth.toInt());
    }
    att.setColor(o.value(QLatin1String("color")).toString());
    return att;
}

int MessageAttachment::imageWidth() const
{
    return mImageWidth;
}

void MessageAttachment::setImageWidth(int imageWidth)
{
    mImageWidth = imageWidth;
}

int MessageAttachment::imageHeight() const
{
    return mImageHeight;
}

void MessageAttachment::setImageHeight(int imageHeight)
{
    mImageHeight = imageHeight;
}

QString MessageAttachment::color() const
{
    return mColor;
}

void MessageAttachment::setColor(const QString &color)
{
    mColor = color;
}

QString MessageAttachment::authorName() const
{
    return mAuthorName;
}

void MessageAttachment::setAuthorName(const QString &authorName)
{
    mAuthorName = authorName;
}

bool MessageAttachment::isEmpty() const
{
    return mLink.isEmpty();
}

bool MessageAttachment::canDownloadAttachment() const
{
    //Improve it !
    if (mLink.startsWith(QLatin1String("http://")) || mLink.startsWith(QLatin1String("https://"))) {
        return false;
    }
    return true;
}

QString MessageAttachment::imageTitle() const
{
    return QStringLiteral("%1 <a href=\'%2'>%2</a>").arg(i18n("File send:")).arg(title());
}

QString MessageAttachment::mimeType() const
{
    return mMimeType;
}

void MessageAttachment::setMimeType(const QString &type)
{
    mMimeType = type;
    mIsAnimatedImage = (mMimeType == QLatin1String("image/gif"));
}

bool MessageAttachment::isAnimatedImage() const
{
    return mIsAnimatedImage;
}

QString MessageAttachment::displayTitle() const
{
    if (canDownloadAttachment()) {
        return i18n("File send: %1", title());
    }
    return QStringLiteral("<a href=\'%1'>%2</a>").arg(link()).arg(title());
}

QString MessageAttachment::description() const
{
    return mDescription;
}

void MessageAttachment::setDescription(const QString &description)
{
    mDescription = description;
}

QString MessageAttachment::title() const
{
    return mTitle;
}

void MessageAttachment::setTitle(const QString &title)
{
    mTitle = title;
}

QString MessageAttachment::link() const
{
    return mLink;
}

void MessageAttachment::setLink(const QString &link)
{
    mLink = link;
}

bool MessageAttachment::operator==(const MessageAttachment &other) const
{
    return (mDescription == other.description())
           && (mTitle == other.title())
           && (mLink == other.link())
           && (mColor == other.color())
           && (mImageHeight == other.imageHeight())
           && (mImageWidth == other.imageWidth())
           && (mAuthorName == other.authorName())
            && (mMimeType == other.mimeType());
}

QDebug operator <<(QDebug d, const MessageAttachment &t)
{
    d << "Title : " << t.title();
    d << "Description: " << t.description();
    d << "Link: " << t.link();
    d << "image dimension: width: " << t.imageWidth() << " height: " << t.imageHeight();
    d << "color: " << t.color();
    d << "authorname: " << t.authorName();
    d << "mimeType: " << t.mimeType();
    return d;
}
