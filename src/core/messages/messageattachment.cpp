/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachment.h"
#include "ruqolaglobalconfig.h"

#include <KIO/Global>
#include <KLocalizedString>
#include <QJsonArray>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
MessageAttachment::MessageAttachment() = default;

void MessageAttachment::parseAttachment(const QJsonObject &attachment)
{
    const QJsonValue description = attachment.value("description"_L1);
    if (!description.isUndefined()) {
        setDescription(description.toString());
    }
    const QJsonValue title = attachment.value("title"_L1);
    if (!title.isUndefined()) {
        setTitle(title.toString());
    }

    const QJsonValue format = attachment.value("format"_L1);
    if (!format.isUndefined()) {
        setFormat(format.toString());
    }

    AttachmentType attType = AttachmentType::Unknown;
    if (attachment.contains("audio_url"_L1)) {
        setLink(attachment.value("audio_url"_L1).toString());
        attType = AttachmentType::Audio;
        setAttachmentSize(attachment.value("audio_size"_L1).toInteger(-1));
    } else if (attachment.contains("video_url"_L1)) {
        setLink(attachment.value("video_url"_L1).toString());
        attType = AttachmentType::Video;
        setAttachmentSize(attachment.value("video_size"_L1).toInteger(-1));
    } else if (attachment.contains("image_url"_L1)) {
        // prefer the title_link as the image_url may just serve us the tiny preview image
        setLink(attachment.value("title_link"_L1).toString());
        mImageUrlPreview = attachment.value("image_url"_L1).toString();
        if (link().isEmpty()) { // fallback to the image_url otherwise
            setLink(mImageUrlPreview);
        }
        attType = AttachmentType::Image;
        // Image Size
        setAttachmentSize(attachment.value("image_size"_L1).toInteger(-1));
    } else if (attachment.contains("author_link"_L1)) {
        setLink(attachment.value("author_link"_L1).toString());
        attType = AttachmentType::NormalText;
        // TODO implement it.
        //    } else if (attachment.contains("message_link"_L1)) {
        //        setLink(attachment.value("message_link"_L1).toString());
        //        attType = AttachmentType::NormalText;
    } else if (attachment.contains("title_link"_L1)) { // Last as an image_url can have a title_link
        setLink(attachment.value("title_link"_L1).toString());
        attType = AttachmentType::File;
    }
    // Add image dimension
    if (attType == AttachmentType::Image) {
        const QJsonValue imageDimensions = attachment.value("image_dimensions"_L1);
        if (!imageDimensions.isUndefined()) {
            const QJsonObject imageDimensionsParams = imageDimensions.toObject();

            setImageHeight(imageDimensionsParams.value("height"_L1).toInt());
            setImageWidth(imageDimensionsParams.value("width"_L1).toInt());
            // TODO validate image size
        } else {
            // Use default value
            setImageHeight(120);
            setImageWidth(120);
        }
    }

    const QJsonValue sizeAttachment = attachment.value("size"_L1);
    if (!sizeAttachment.isUndefined()) {
        setAttachmentSize(sizeAttachment.toInteger(-1));
    }

    setAuthorName(attachment.value("author_name"_L1).toString());
    setAuthorIcon(attachment.value("author_icon"_L1).toString());

    // Color
    const QJsonValue color = attachment.value("color"_L1);
    if (!color.isUndefined()) {
        setColor(color.toString());
    }
    // MimeType
    setMimeType(attachment.value("image_type"_L1).toString());

    // Text
    const QJsonValue text = attachment.value("text"_L1);
    if (!text.isUndefined()) {
        const QJsonValue messagelink = attachment.value("message_link"_L1);
        if (messagelink.isUndefined()) { // Don't show attachment if we have message_link. We already implement message preview
            attType = AttachmentType::NormalText;
            setText(text.toString());
        }
    }
    const QJsonArray fieldsArray = attachment.value("fields"_L1).toArray();
    QList<MessageAttachmentField> messageFields;
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
    mCollapsed = attachment.value("collapsed"_L1).toBool();
    if (attachment.contains("actions"_L1)) {
        qDebug() << " att action " << attachment.value("actions"_L1);
        // TODO add actions
    }

    generateTitle();
}

QJsonObject MessageAttachment::serialize(const MessageAttachment &messageAttach)
{
    QJsonObject obj;
    if (!messageAttach.description().isEmpty()) {
        obj["description"_L1] = messageAttach.description();
    }
    if (!messageAttach.title().isEmpty()) {
        obj["title"_L1] = messageAttach.title();
    }
    if (const QString url = messageAttach.link(); !url.isEmpty()) {
        obj["url"_L1] = url;
    }
    if (!messageAttach.imageUrlPreview().isEmpty()) {
        obj["image_preview"_L1] = messageAttach.imageUrlPreview();
    }

    const QString authorname = messageAttach.authorName();
    if (!authorname.isEmpty()) {
        obj["authorname"_L1] = authorname;
    }
    const QString authorIcon = messageAttach.authorIcon();
    if (!authorIcon.isEmpty()) {
        obj["authoricon"_L1] = authorIcon;
    }
    const QString mimeType = messageAttach.mimeType();
    if (!mimeType.isEmpty()) {
        obj["mimetype"_L1] = mimeType;
    }
    if ((messageAttach.imageHeight() != -1) && (messageAttach.imageWidth() != -1)) {
        obj["image_height"_L1] = messageAttach.imageHeight();
        obj["image_width"_L1] = messageAttach.imageWidth();
    }
    const QString color = messageAttach.color();
    if (!color.isEmpty()) {
        obj["color"_L1] = color;
    }
    const QString text = messageAttach.text();
    if (!text.isEmpty()) {
        obj["text"_L1] = text;
    }
    const QString format = messageAttach.format();
    if (!format.isEmpty()) {
        obj["format"_L1] = format;
    }
    if (messageAttach.attachmentSize() != -1) {
        obj["attachment_size"_L1] = messageAttach.attachmentSize();
    }
    QJsonArray fieldArray;
    for (int i = 0, total = messageAttach.attachmentFields().count(); i < total; ++i) {
        const QJsonObject fields = MessageAttachmentField::serialize(messageAttach.attachmentFields().at(i));
        fieldArray.append(fields);
    }
    if (!fieldArray.isEmpty()) {
        obj["fields"_L1] = fieldArray;
    }
    if (messageAttach.collapsed()) {
        obj["collapsed"_L1] = true;
    }
    obj["attachmentType"_L1] = QJsonValue::fromVariant(QVariant::fromValue<MessageAttachment::AttachmentType>(messageAttach.attachmentType()));
    if (const bool show = messageAttach.showAttachment()) {
        obj["show_attachment"_L1] = show;
    }
    // TODO add mMessageAttachmentAction
    return obj;
}

MessageAttachment MessageAttachment::deserialize(const QJsonObject &o)
{
    MessageAttachment att;
    att.setDescription(o.value("description"_L1).toString());
    att.setTitle(o.value("title"_L1).toString());
    att.setText(o.value("text"_L1).toString());
    att.setLink(o.value("url"_L1).toString());
    att.setImageUrlPreview(o.value("image_preview"_L1).toString());
    att.setAuthorName(o.value("authorname"_L1).toString());
    att.setAuthorIcon(o.value("authoricon"_L1).toString());
    att.setMimeType(o.value("mimetype"_L1).toString());
    att.setAttachmentSize(o.value("attachment_size"_L1).toInteger(-1));
    att.setFormat(o.value("format"_L1).toString());

    const QJsonValue valHeight = o.value("image_height"_L1);
    if (!valHeight.isUndefined()) {
        att.setImageHeight(valHeight.toInt());
    }
    const QJsonValue valWidth = o.value("image_width"_L1);
    if (!valWidth.isUndefined()) {
        att.setImageWidth(valWidth.toInt());
    }
    att.setColor(o.value("color"_L1).toString());
    const QJsonArray fieldsArray = o.value("fields"_L1).toArray();
    QList<MessageAttachmentField> messageFields;
    messageFields.reserve(fieldsArray.size());
    for (int i = 0, total = fieldsArray.size(); i < total; ++i) {
        messageFields.append(MessageAttachmentField::deserialize(fieldsArray.at(i).toObject()));
    }
    att.setAttachmentFields(messageFields);
    att.setCollapsed(o.value("collapsed"_L1).toBool());
    att.setAttachmentType(o["attachmentType"_L1].toVariant().value<AttachmentType>());
    att.setShowAttachment(o["show_attachment"_L1].toBool());
    // TODO add mMessageAttachmentAction
    att.generateTitle();
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
    if (mLink.startsWith("http://"_L1) || mLink.startsWith("https://"_L1)) {
        return false;
    }
    return true;
}

QString MessageAttachment::mimeType() const
{
    return mMimeType;
}

void MessageAttachment::setMimeType(const QString &type)
{
    mMimeType = type;
    if (mMimeType == "image/gif"_L1) {
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
    if (mAttachmentType == AttachmentType::Image) {
        // By default use false for showing it or using settings for image
        mShowAttachment = RuqolaGlobalConfig::self()->showImage();
    }
}

QList<MessageAttachmentField> MessageAttachment::attachmentFields() const
{
    return mAttachmentFields;
}

void MessageAttachment::setAttachmentFields(const QList<MessageAttachmentField> &attachmentFields)
{
    mAttachmentFields = attachmentFields;
    generateAttachmentFieldsText();
}

QString MessageAttachment::fixTitle(const QString &title) const
{
    QString newTitle = title;
    if (newTitle.length() > 80) {
        newTitle = newTitle.left(80) + u"..."_s;
    }
    return newTitle;
}

bool MessageAttachment::hasMessageAttachmentActions() const
{
    return mMessageAttachmentActions.isValid();
}

MessageAttachmentActions MessageAttachment::messageAttachmentActions() const
{
    return mMessageAttachmentActions;
}

void MessageAttachment::setMessageAttachmentActions(const MessageAttachmentActions &newMessageAttachmentAction)
{
    mMessageAttachmentActions = newMessageAttachmentAction;
}

QString MessageAttachment::format() const
{
    return mFormat;
}

void MessageAttachment::setFormat(const QString &newFormat)
{
    mFormat = newFormat;
}

void MessageAttachment::generateTitle()
{
    if (!mTitle.isEmpty()) {
        if (mAttachmentGeneratedTitle.isEmpty()) {
            if (mAttachmentSize == -1) {
                mAttachmentGeneratedTitle = fixTitle(mTitle);
            } else {
                mAttachmentGeneratedTitle = u"%1 (%2)"_s.arg(fixTitle(mTitle), KIO::convertSize(mAttachmentSize));
            }
        }
    }
}

QString MessageAttachment::attachmentGeneratedTitle() const
{
    return mAttachmentGeneratedTitle;
}

void MessageAttachment::generateAttachmentFieldsText()
{
    if (mAttachmentFields.isEmpty()) {
        mAttachmentFieldsText.clear();
    } else {
        const QString formatFirstCell = QStringLiteral(R"( padding-left: 5px; border-left: 5px solid %1)").arg(color());
        QString result = QStringLiteral(R"(<qt><table style="border-collapse: collapse; border-spacing: 0;" width="100%" align="center"><tr>)");
        QStringList values;
        values.reserve(mAttachmentFields.count());
        int i = 0;
        for (const MessageAttachmentField &field : std::as_const(mAttachmentFields)) {
            result += u"<td style=\"text-align: left;%2\"><b>%1</b></td>"_s.arg(field.title(), (i == 0) ? formatFirstCell : QString());
            values << field.value();
            ++i;
        }
        result += u"</tr><tr>"_s;
        i = 0;
        for (const QString &res : std::as_const(values)) {
            result += u"<td style=\"text-align: left;%2\">%1</td>"_s.arg(res, (i == 0) ? formatFirstCell : QString());
            ++i;
        }
        result += u"</tr></table></qt>"_s;
        mAttachmentFieldsText += result;
    }
}

qint64 MessageAttachment::attachmentSize() const
{
    return mAttachmentSize;
}

void MessageAttachment::setAttachmentSize(qint64 newAttachmentSize)
{
    mAttachmentSize = newAttachmentSize;
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

QByteArray MessageAttachment::attachmentId() const
{
    return mAttachmentId;
}

void MessageAttachment::setAttachmentId(const QByteArray &attachementId)
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

bool MessageAttachment::hasDescription() const
{
    return !mDescription.isEmpty();
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
    if (link.endsWith(".gif"_L1)) { // Gify doesn't set mimetype
        mIsAnimatedImage = true;
    } else {
        QUrl url(link);
        if (url.fileName().endsWith(QLatin1StringView(
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
        && (mAuthorIcon == other.authorIcon()) && (mImageUrlPreview == other.imageUrlPreview()) && (mAttachmentSize == other.attachmentSize())
        && (mAttachmentGeneratedTitle == other.attachmentGeneratedTitle()) && (mFormat == other.format())
        && (mMessageAttachmentActions == other.messageAttachmentActions());
}

QDebug operator<<(QDebug d, const MessageAttachment &t)
{
    d.space() << "Title :" << t.title();
    d.space() << "Description:" << t.description();
    d.space() << "Link:" << t.link();
    d.space() << "image dimension: width:" << t.imageWidth() << " height: " << t.imageHeight();
    d.space() << "color:" << t.color();
    d.space() << "authorname:" << t.authorName();
    d.space() << "mimeType:" << t.mimeType();
    d.space() << "text:" << t.text();
    d.space() << "collapsed" << t.collapsed();
    d.space() << "attachmentfields" << t.attachmentFields();
    d.space() << "showAttachment" << t.showAttachment();
    d.space() << "AttachmentType:" << t.attachmentType();
    d.space() << "mAuthorIcon:" << t.authorIcon();
    d.space() << "imageUrlPreview" << t.imageUrlPreview();
    d.space() << "attachment size" << t.attachmentSize();
    d.space() << "attachmentGeneratedTitle" << t.attachmentGeneratedTitle();
    d.space() << "format" << t.format();
    d.space() << "mMessageAttachmentActions" << t.messageAttachmentActions();
    return d;
}

#include "moc_messageattachment.cpp"
