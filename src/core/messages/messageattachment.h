/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

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

    Q_REQUIRED_RESULT bool isValid() const;

    Q_REQUIRED_RESULT QString description() const;
    void setDescription(const QString &description);

    Q_REQUIRED_RESULT QString title() const;
    void setTitle(const QString &title);

    Q_REQUIRED_RESULT QString link() const;
    void setLink(const QString &link);

    Q_REQUIRED_RESULT bool operator==(const MessageAttachment &other) const;

    static QJsonObject serialize(const MessageAttachment &message);
    static MessageAttachment fromJson(const QJsonObject &o);
    Q_REQUIRED_RESULT int imageWidth() const;
    void setImageWidth(int imageWidth);

    Q_REQUIRED_RESULT int imageHeight() const;
    void setImageHeight(int imageHeight);

    Q_REQUIRED_RESULT QString color() const;
    void setColor(const QString &color);

    Q_REQUIRED_RESULT QString authorName() const;
    void setAuthorName(const QString &authorName);

    Q_REQUIRED_RESULT bool canDownloadAttachment() const;
    Q_REQUIRED_RESULT QString displayTitle() const;
    Q_REQUIRED_RESULT QString imageTitle() const;

    Q_REQUIRED_RESULT QString mimeType() const;
    void setMimeType(const QString &type);

    Q_REQUIRED_RESULT bool isAnimatedImage() const;

    Q_REQUIRED_RESULT QString text() const;
    void setText(const QString &text);

    Q_REQUIRED_RESULT AttachmentType attachmentType() const;
    void setAttachmentType(AttachmentType attachmentType);

    Q_REQUIRED_RESULT QVector<MessageAttachmentField> attachmentFields() const;
    void setAttachmentFields(const QVector<MessageAttachmentField> &attachmentFields);

    Q_REQUIRED_RESULT bool collapsed() const;
    void setCollapsed(bool collapsed);

    Q_REQUIRED_RESULT QString attachmentId() const;
    void setAttachmentId(const QString &attachementId);

    Q_REQUIRED_RESULT bool showAttachment() const;
    void setShowAttachment(bool showAttachment);

    Q_REQUIRED_RESULT QString authorIcon() const;
    void setAuthorIcon(const QString &authorIcon);

    Q_REQUIRED_RESULT QString attachmentFieldsText() const;

    void setImageUrlPreview(const QString &newImageUrlPreview);

    Q_REQUIRED_RESULT const QString &imageUrlPreview() const;

private:
    void generateAttachmentFieldsText();
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
Q_DECLARE_TYPEINFO(MessageAttachment, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const MessageAttachment &t);
