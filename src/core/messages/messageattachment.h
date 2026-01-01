/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "messageattachmentfield.h"
#include <QList>
class MessageAttachmentField;
class LIBRUQOLACORE_EXPORT MessageAttachment
{
    Q_GADGET
public:
    MessageAttachment();

    enum class AttachmentType : uint8_t {
        Unknown,
        NormalText,
        File,
        Video,
        Audio,
        Image,
    };
    Q_ENUM(AttachmentType)
    void parseAttachment(const QJsonObject &o);

    [[nodiscard]] bool isValid() const;

    [[nodiscard]] QString description() const;
    void setDescription(const QString &description);

    [[nodiscard]] QString title() const;
    void setTitle(const QString &title);

    [[nodiscard]] QString link() const;
    void setLink(const QString &link);

    [[nodiscard]] bool operator==(const MessageAttachment &other) const;

    [[nodiscard]] static QJsonObject serialize(const MessageAttachment &message);
    [[nodiscard]] static MessageAttachment deserialize(const QJsonObject &o);
    [[nodiscard]] int imageWidth() const;
    void setImageWidth(int imageWidth);

    [[nodiscard]] int imageHeight() const;
    void setImageHeight(int imageHeight);

    [[nodiscard]] QString color() const;
    void setColor(const QString &color);

    [[nodiscard]] QString authorName() const;
    void setAuthorName(const QString &authorName);

    [[nodiscard]] bool canDownloadAttachment() const;

    [[nodiscard]] QString mimeType() const;
    void setMimeType(const QString &type);

    [[nodiscard]] bool isAnimatedImage() const;

    [[nodiscard]] QString text() const;
    void setText(const QString &text);

    [[nodiscard]] AttachmentType attachmentType() const;
    void setAttachmentType(AttachmentType attachmentType);

    [[nodiscard]] QList<MessageAttachmentField> attachmentFields() const;
    void setAttachmentFields(const QList<MessageAttachmentField> &attachmentFields);

    [[nodiscard]] bool collapsed() const;
    void setCollapsed(bool collapsed);

    [[nodiscard]] QByteArray attachmentId() const;
    void setAttachmentId(const QByteArray &attachementId);

    [[nodiscard]] bool showAttachment() const;
    void setShowAttachment(bool showAttachment);

    [[nodiscard]] QString authorIcon() const;
    void setAuthorIcon(const QString &authorIcon);

    [[nodiscard]] QString attachmentFieldsText() const;

    void setImageUrlPreview(const QString &newImageUrlPreview);

    [[nodiscard]] const QString &imageUrlPreview() const;

    [[nodiscard]] bool hasDescription() const;

    [[nodiscard]] qint64 attachmentSize() const;
    void setAttachmentSize(qint64 newAttachmentSize);

    [[nodiscard]] QString attachmentGeneratedTitle() const;
    void generateTitle();

    [[nodiscard]] QString format() const;
    void setFormat(const QString &newFormat);

private:
    LIBRUQOLACORE_NO_EXPORT void generateAttachmentFieldsText();
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString fixTitle(const QString &title) const;
    QList<MessageAttachmentField> mAttachmentFields;
    AttachmentType mAttachmentType = AttachmentType::Unknown;
    QString mColor;
    QString mDescription;
    QString mTitle;
    QString mLink;
    QString mAuthorName;
    QString mMimeType;
    QString mText;
    QByteArray mAttachmentId;
    QString mAuthorIcon;
    QString mAttachmentFieldsText;
    QString mImageUrlPreview;
    QString mAttachmentGeneratedTitle;
    QString mFormat;
    qint64 mAttachmentSize = -1;
    int mImageHeight = -1;
    int mImageWidth = -1;
    bool mIsAnimatedImage = false;
    bool mCollapsed = false;
    bool mShowAttachment = false;
};
Q_DECLARE_TYPEINFO(MessageAttachment, Q_RELOCATABLE_TYPE);
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const MessageAttachment &t);
