/*
   Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>

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
#include <QJsonArray>
#include <QJsonObject>

MessageAttachment::MessageAttachment()
{
}

void MessageAttachment::parseAttachment(const QJsonObject &attachment)
{
    const QJsonValue description = attachment.value(QLatin1String("description"));
    if (!description.isUndefined()) {
        setDescription(description.toString());
    }
    const QJsonValue title = attachment.value(QLatin1String("title"));
    if (!title.isUndefined()) {
        setTitle(title.toString());
    }

    if (attachment.contains(QLatin1String("audio_url"))) {
        setLink(attachment.value(QLatin1String("audio_url")).toString());
        mAttachmentType = AttachmentType::Audio;
    } else if (attachment.contains(QLatin1String("video_url"))) {
        setLink(attachment.value(QLatin1String("video_url")).toString());
        mAttachmentType = AttachmentType::Video;
    } else if (attachment.contains(QLatin1String("image_url"))) {
        setLink(attachment.value(QLatin1String("image_url")).toString());
        mAttachmentType = AttachmentType::Image;
    } else if (attachment.contains(QLatin1String("title_link"))) { //Last as an image_url can have a title_link
        setLink(attachment.value(QLatin1String("title_link")).toString());
        mAttachmentType = AttachmentType::File;
    }
    //Add image dimension
    if (mAttachmentType == AttachmentType::Image) {
        const QJsonValue imageDimensions = attachment.value(QLatin1String("image_dimensions"));
        if (!imageDimensions.isUndefined()) {
            const QJsonObject imageDimensionsParams = imageDimensions.toObject();

            setImageHeight(imageDimensionsParams.value(QLatin1String("height")).toInt());
            setImageWidth(imageDimensionsParams.value(QLatin1String("width")).toInt());
            //TODO validate image size
        } else {
            //Use default value
            setImageHeight(120);
            setImageWidth(120);
        }
    }

    setAuthorName(attachment.value(QLatin1String("author_name")).toString());
    //Color
    const QJsonValue color = attachment.value(QLatin1String("color"));
    if (!color.isUndefined()) {
        setColor(color.toString());
    }
    //MimeType
    setMimeType(attachment.value(QLatin1String("image_type")).toString());

    //Text
    const QJsonValue text = attachment.value(QLatin1String("text"));
    if (!text.isUndefined()) {
        const QJsonValue messagelink = attachment.value(QLatin1String("message_link"));
        if (messagelink.isUndefined()) { //Don't show attachment if we have message_link. We already implement message preview
            mAttachmentType = AttachmentType::NormalText;
            setText(text.toString());
        }
    }
    const QJsonArray fieldsArray = attachment.value(QLatin1String("fields")).toArray();
    QVector<MessageAttachmentField> messageFields;
    messageFields.reserve(fieldsArray.size());
    for (int i = 0, total = fieldsArray.size(); i < total; ++i) {
        messageFields.append(MessageAttachmentField::fromJson(fieldsArray.at(i).toObject()));
    }
    if (!messageFields.isEmpty()) {
        setAttachmentFields(messageFields);
        if (mAttachmentType == AttachmentType::Unknown) {
            mAttachmentType = AttachmentType::NormalText;
        }
    }
    mCollapsed = attachment.value(QLatin1String("collapsed")).toBool();
}

QJsonObject MessageAttachment::serialize(const MessageAttachment &message)
{
    QJsonObject obj;
    if (!message.description().isEmpty()) {
        obj[QStringLiteral("description")] = message.description();
    }
    if (!message.title().isEmpty()) {
        obj[QStringLiteral("title")] = message.title();
    }
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
    const QString text = message.text();
    if (!text.isEmpty()) {
        obj[QStringLiteral("text")] = text;
    }

    QJsonArray fieldArray;
    for (int i = 0; i < message.attachmentFields().count(); ++i) {
        QJsonObject fields = MessageAttachmentField::serialize(message.attachmentFields().at(i));
        fieldArray.append(fields);
    }
    if (!fieldArray.isEmpty()) {
        obj[QStringLiteral("fields")] = fieldArray;
    }
    if (message.collapsed()) {
        obj[QStringLiteral("collapsed")] = true;
    }
    return obj;
}

MessageAttachment MessageAttachment::fromJson(const QJsonObject &o)
{
    MessageAttachment att;
    att.setDescription(o.value(QLatin1String("description")).toString());
    att.setTitle(o.value(QLatin1String("title")).toString());
    att.setText(o.value(QLatin1String("text")).toString());
    att.setLink(o.value(QLatin1String("url")).toString());
    att.setAuthorName(o.value(QLatin1String("authorname")).toString());
    att.setMimeType(o.value(QLatin1String("mimetype")).toString());
    const QJsonValue valHeight = o.value(QLatin1String("image_height"));
    if (!valHeight.isUndefined()) {
        att.setImageHeight(valHeight.toInt());
    }
    const QJsonValue valWidth = o.value(QLatin1String("image_width"));
    if (!valWidth.isUndefined()) {
        att.setImageWidth(valWidth.toInt());
    }
    att.setColor(o.value(QLatin1String("color")).toString());
    const QJsonArray fieldsArray = o.value(QLatin1String("fields")).toArray();
    QVector<MessageAttachmentField> messageFields;
    messageFields.reserve(fieldsArray.size());
    for (int i = 0, total = fieldsArray.size(); i < total; ++i) {
        messageFields.append(MessageAttachmentField::fromJson(fieldsArray.at(i).toObject()));
    }
    att.setAttachmentFields(messageFields);
    att.setCollapsed(o.value(QLatin1String("collapsed")).toBool());
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

bool MessageAttachment::isValid() const
{
    return !mLink.isEmpty() || !mText.isEmpty();
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
    return QStringLiteral("%1 <a href=\'%2'>%2</a>").arg(i18n("File Uploaded:"), title());
}

QString MessageAttachment::mimeType() const
{
    return mMimeType;
}

void MessageAttachment::setMimeType(const QString &type)
{
    mMimeType = type;
    if (mMimeType == QLatin1String("image/gif")) {
        mIsAnimatedImage = true;
    }
}

bool MessageAttachment::isAnimatedImage() const
{
    return mIsAnimatedImage;
}

QString MessageAttachment::text() const
{
    return mText;
}

void MessageAttachment::setText(const QString &text)
{
    mText = text;
}

MessageAttachment::AttachmentType MessageAttachment::attachmentType() const
{
    return mAttachmentType;
}

void MessageAttachment::setAttachmentType(AttachmentType attachmentType)
{
    mAttachmentType = attachmentType;
}

QVector<MessageAttachmentField> MessageAttachment::attachmentFields() const
{
    return mAttachmentFields;
}

void MessageAttachment::setAttachmentFields(const QVector<MessageAttachmentField> &attachmentFields)
{
    mAttachmentFields = attachmentFields;
}

bool MessageAttachment::collapsed() const
{
    return mCollapsed;
}

void MessageAttachment::setCollapsed(bool collapsed)
{
    mCollapsed = collapsed;
}

QString MessageAttachment::displayTitle() const
{
    if (canDownloadAttachment()) {
        return i18n("File Uploaded: %1", title());
    }
    return QStringLiteral("<a href=\'%1'>%2</a>").arg(link(), title());
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
    if (link.endsWith(QLatin1String(".gif"))) { // Gify doesn't set mimetype
        mIsAnimatedImage = true;
    }
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
           && (mMimeType == other.mimeType())
           && (mText == other.text())
           && (mAttachmentFields == other.attachmentFields())
           && (mCollapsed == other.collapsed());
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
    d << "text: " << t.text();
    d << "collapsed " << t.collapsed();
    d << "attachmentfields " << t.attachmentFields();
    return d;
}
