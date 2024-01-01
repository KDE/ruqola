/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachment.h"
#include "ruqolaglobalconfig.h"

#include <KLocalizedString>
#include <QJsonArray>
#include <QJsonObject>

MessageAttachment::MessageAttachment() = default;

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
    AttachmentType attType = AttachmentType::Unknown;
    if (attachment.contains(QLatin1String("audio_url"))) {
        setLink(attachment.value(QLatin1String("audio_url")).toString());
        attType = AttachmentType::Audio;
    } else if (attachment.contains(QLatin1String("video_url"))) {
        setLink(attachment.value(QLatin1String("video_url")).toString());
        attType = AttachmentType::Video;
    } else if (attachment.contains(QLatin1String("image_url"))) {
        // prefer the title_link as the image_url may just serve us the tiny preview image
        setLink(attachment.value(QLatin1String("title_link")).toString());
        mImageUrlPreview = attachment.value(QLatin1String("image_url")).toString();
        if (link().isEmpty()) // fallback to the image_url otherwise
            setLink(mImageUrlPreview);
        attType = AttachmentType::Image;
    } else if (attachment.contains(QLatin1String("author_link"))) {
        setLink(attachment.value(QLatin1String("author_link")).toString());
        attType = AttachmentType::NormalText;
        // TODO implement it.
        //    } else if (attachment.contains(QLatin1String("message_link"))) {
        //        setLink(attachment.value(QLatin1String("message_link")).toString());
        //        attType = AttachmentType::NormalText;
    } else if (attachment.contains(QLatin1String("title_link"))) { // Last as an image_url can have a title_link
        setLink(attachment.value(QLatin1String("title_link")).toString());
        attType = AttachmentType::File;
    }
    // Add image dimension
    if (attType == AttachmentType::Image) {
        const QJsonValue imageDimensions = attachment.value(QLatin1String("image_dimensions"));
        if (!imageDimensions.isUndefined()) {
            const QJsonObject imageDimensionsParams = imageDimensions.toObject();

            setImageHeight(imageDimensionsParams.value(QLatin1String("height")).toInt());
            setImageWidth(imageDimensionsParams.value(QLatin1String("width")).toInt());
            // TODO validate image size
        } else {
            // Use default value
            setImageHeight(120);
            setImageWidth(120);
        }
    }

    setAuthorName(attachment.value(QLatin1String("author_name")).toString());
    setAuthorIcon(attachment.value(QLatin1String("author_icon")).toString());
    // Color
    const QJsonValue color = attachment.value(QLatin1String("color"));
    if (!color.isUndefined()) {
        setColor(color.toString());
    }
    // MimeType
    setMimeType(attachment.value(QLatin1String("image_type")).toString());

    // Text
    const QJsonValue text = attachment.value(QLatin1String("text"));
    if (!text.isUndefined()) {
        const QJsonValue messagelink = attachment.value(QLatin1String("message_link"));
        if (messagelink.isUndefined()) { // Don't show attachment if we have message_link. We already implement message preview
            attType = AttachmentType::NormalText;
            setText(text.toString());
        }
    }
    const QJsonArray fieldsArray = attachment.value(QLatin1String("fields")).toArray();
    QVector<MessageAttachmentField> messageFields;
    messageFields.reserve(fieldsArray.size());
    for (int i = 0, total = fieldsArray.size(); i < total; ++i) {
        messageFields.append(MessageAttachmentField::deserialize(fieldsArray.at(i).toObject()));
    }
    if (!messageFields.isEmpty()) {
        setAttachmentFields(messageFields);
        if (attType == AttachmentType::Unknown) {
            attType = AttachmentType::NormalText;
        }
    }
    setAttachmentType(attType);
    mCollapsed = attachment.value(QLatin1String("collapsed")).toBool();
}

QJsonObject MessageAttachment::serialize(const MessageAttachment &messageAttach)
{
    QJsonObject obj;
    if (!messageAttach.description().isEmpty()) {
        obj[QLatin1String("description")] = messageAttach.description();
    }
    if (!messageAttach.title().isEmpty()) {
        obj[QLatin1String("title")] = messageAttach.title();
    }
    obj[QLatin1String("url")] = messageAttach.link();
    if (!messageAttach.imageUrlPreview().isEmpty()) {
        obj[QLatin1String("image_preview")] = messageAttach.imageUrlPreview();
    }

    const QString authorname = messageAttach.authorName();
    if (!authorname.isEmpty()) {
        obj[QLatin1String("authorname")] = authorname;
    }
    const QString authorIcon = messageAttach.authorIcon();
    if (!authorIcon.isEmpty()) {
        obj[QLatin1String("authoricon")] = authorIcon;
    }
    const QString mimeType = messageAttach.mimeType();
    if (!mimeType.isEmpty()) {
        obj[QLatin1String("mimetype")] = mimeType;
    }
    if ((messageAttach.imageHeight() != -1) && (messageAttach.imageWidth() != -1)) {
        obj[QLatin1String("image_height")] = messageAttach.imageHeight();
        obj[QLatin1String("image_width")] = messageAttach.imageWidth();
    }
    const QString color = messageAttach.color();
    if (!color.isEmpty()) {
        obj[QLatin1String("color")] = color;
    }
    const QString text = messageAttach.text();
    if (!text.isEmpty()) {
        obj[QLatin1String("text")] = text;
    }

    QJsonArray fieldArray;
    for (int i = 0, total = messageAttach.attachmentFields().count(); i < total; ++i) {
        const QJsonObject fields = MessageAttachmentField::serialize(messageAttach.attachmentFields().at(i));
        fieldArray.append(std::move(fields));
    }
    if (!fieldArray.isEmpty()) {
        obj[QLatin1String("fields")] = fieldArray;
    }
    if (messageAttach.collapsed()) {
        obj[QLatin1String("collapsed")] = true;
    }
    obj[QLatin1String("attachmentType")] = QJsonValue::fromVariant(QVariant::fromValue<MessageAttachment::AttachmentType>(messageAttach.attachmentType()));
    if (bool show = messageAttach.showAttachment()) {
        obj[QLatin1String("show_attachment")] = show;
    }
    return obj;
}

MessageAttachment MessageAttachment::deserialize(const QJsonObject &o)
{
    MessageAttachment att;
    att.setDescription(o.value(QLatin1String("description")).toString());
    att.setTitle(o.value(QLatin1String("title")).toString());
    att.setText(o.value(QLatin1String("text")).toString());
    att.setLink(o.value(QLatin1String("url")).toString());
    att.setImageUrlPreview(o.value(QLatin1String("image_preview")).toString());
    att.setAuthorName(o.value(QLatin1String("authorname")).toString());
    att.setAuthorIcon(o.value(QLatin1String("authoricon")).toString());
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
        messageFields.append(MessageAttachmentField::deserialize(fieldsArray.at(i).toObject()));
    }
    att.setAttachmentFields(messageFields);
    att.setCollapsed(o.value(QLatin1String("collapsed")).toBool());
    att.setAttachmentType(o[QLatin1String("attachmentType")].toVariant().value<AttachmentType>());
    att.setShowAttachment(o[QLatin1String("show_attachment")].toBool());
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
    return !mAttachmentId.isEmpty() && (!mLink.isEmpty() || !mText.isEmpty() || !mAttachmentFields.isEmpty());
}

bool MessageAttachment::canDownloadAttachment() const
{
    // Improve it !
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
    if (mAttachmentType == Image) {
        // By default use false for showing it or using settings for image
        mShowAttachment = RuqolaGlobalConfig::self()->showImage();
    }
}

QVector<MessageAttachmentField> MessageAttachment::attachmentFields() const
{
    return mAttachmentFields;
}

void MessageAttachment::setAttachmentFields(const QVector<MessageAttachmentField> &attachmentFields)
{
    mAttachmentFields = attachmentFields;
    generateAttachmentFieldsText();
}

void MessageAttachment::generateAttachmentFieldsText()
{
    QString result = QStringLiteral(R"(<qt><table width="100%"><tr>)");
    QStringList values;
    values.reserve(mAttachmentFields.count());
    for (const MessageAttachmentField &field : std::as_const(mAttachmentFields)) {
        result += QStringLiteral("<th><b>%1</b></th>").arg(field.title());
        values << field.value();
    }
    result += QStringLiteral("</tr><tr>");
    for (const QString &res : std::as_const(values)) {
        result += QStringLiteral("<td>%1</td>").arg(res);
    }
    result += QStringLiteral("</tr></table></qt>");
    mAttachmentFieldsText += result;
}

const QString &MessageAttachment::imageUrlPreview() const
{
    return mImageUrlPreview;
}

void MessageAttachment::setImageUrlPreview(const QString &newImageUrlPreview)
{
    mImageUrlPreview = newImageUrlPreview;
}

QString MessageAttachment::attachmentFieldsText() const
{
    return mAttachmentFieldsText;
}

bool MessageAttachment::collapsed() const
{
    return mCollapsed;
}

void MessageAttachment::setCollapsed(bool collapsed)
{
    mCollapsed = collapsed;
}

QString MessageAttachment::attachmentId() const
{
    return mAttachmentId;
}

void MessageAttachment::setAttachmentId(const QString &attachementId)
{
    mAttachmentId = attachementId;
}

bool MessageAttachment::showAttachment() const
{
    return mShowAttachment;
}

void MessageAttachment::setShowAttachment(bool showAttachment)
{
    mShowAttachment = showAttachment;
}

QString MessageAttachment::authorIcon() const
{
    return mAuthorIcon;
}

void MessageAttachment::setAuthorIcon(const QString &authorIcon)
{
    mAuthorIcon = authorIcon;
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
    } else {
        QUrl url(link);
        if (url.fileName().endsWith(QLatin1String(
                ".gif"))) { // Gify can return
                            // https://media2.giphy.com/media/Id66GDfKacJzxSvhqV/giphy.gif?cid=e1bb72ffh1nt4tll6fw7bab09yqqznaupcxewcw2av5m59yi&rid=giphy.gif&ct=g
            mIsAnimatedImage = true;
        }
    }
}

bool MessageAttachment::operator==(const MessageAttachment &other) const
{
    return (mDescription == other.description()) && (mTitle == other.title()) && (mLink == other.link()) && (mColor == other.color())
        && (mImageHeight == other.imageHeight()) && (mImageWidth == other.imageWidth()) && (mAuthorName == other.authorName())
        && (mMimeType == other.mimeType()) && (mText == other.text()) && (mAttachmentFields == other.attachmentFields()) && (mCollapsed == other.collapsed())
        && (mAuthorIcon == other.authorIcon()) && (mImageUrlPreview == other.imageUrlPreview());
}

QDebug operator<<(QDebug d, const MessageAttachment &t)
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
    d << "showAttachment " << t.showAttachment();
    d << "AttachmentType: " << t.attachmentType();
    d << "mAuthorIcon: " << t.authorIcon();
    d << "imageUrlPreview " << t.imageUrlPreview();
    return d;
}

#include "moc_messageattachment.cpp"
