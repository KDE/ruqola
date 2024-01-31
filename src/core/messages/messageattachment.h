/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "messageattachmentfield.h"
#include <QDebug>
class MessageAttachmentField;
class LIBRUQOLACORE_EXPORT MessageAttachment
{
    Q_GADGET
public:
    MessageAttachment();

    enum AttachmentType {
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

    static QJsonObject serialize(const MessageAttachment &message);
    static MessageAttachment deserialize(const QJsonObject &o);
    [[nodiscard]] int imageWidth() const;
    void setImageWidth(int imageWidth);

    [[nodiscard]] int imageHeight() const;
    void setImageHeight(int imageHeight);

    [[nodiscard]] QString color() const;
    void setColor(const QString &color);

    [[nodiscard]] QString authorName() const;
    void setAuthorName(const QString &authorName);

    [[nodiscard]] bool canDownloadAttachment() const;
    [[nodiscard]] QString displayTitle() const;
    [[nodiscard]] QString imageTitle() const;

    [[nodiscard]] QString mimeType() const;
    void setMimeType(const QString &type);

    [[nodiscard]] bool isAnimatedImage() const;

    [[nodiscard]] QString text() const;
    void setText(const QString &text);

    [[nodiscard]] AttachmentType attachmentType() const;
    void setAttachmentType(AttachmentType attachmentType);

    [[nodiscard]] QVector<MessageAttachmentField> attachmentFields() const;
    void setAttachmentFields(const QVector<MessageAttachmentField> &attachmentFields);

    [[nodiscard]] bool collapsed() const;
    void setCollapsed(bool collapsed);

    [[nodiscard]] QString attachmentId() const;
    void setAttachmentId(const QString &attachementId);

    [[nodiscard]] bool showAttachment() const;
    void setShowAttachment(bool showAttachment);

    [[nodiscard]] QString authorIcon() const;
    void setAuthorIcon(const QString &authorIcon);

    [[nodiscard]] QString attachmentFieldsText() const;

    void setImageUrlPreview(const QString &newImageUrlPreview);

    [[nodiscard]] const QString &imageUrlPreview() const;

    [[nodiscard]] bool hasDescription() const;

private:
    LIBRUQOLACORE_NO_EXPORT void generateAttachmentFieldsText();
    QVector<MessageAttachmentField> mAttachmentFields;
    AttachmentType mAttachmentType = Unknown;
    QString mColor;
    QString mDescription;
    QString mTitle;
    QString mLink;
    QString mAuthorName;
    QString mMimeType;
    QString mText;
    QString mAttachmentId;
    QString mAuthorIcon;
    QString mAttachmentFieldsText;
    QString mImageUrlPreview;
    int mImageHeight = -1;
    int mImageWidth = -1;
    bool mIsAnimatedImage = false;
    bool mCollapsed = false;
    bool mShowAttachment = false;
};
Q_DECLARE_TYPEINFO(MessageAttachment, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const MessageAttachment &t);
