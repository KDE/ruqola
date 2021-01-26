/*
   Copyright (c) 2017-2021 Laurent Montel <montel@kde.org>

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

#ifndef MESSAGEATTACHMENT_H
#define MESSAGEATTACHMENT_H

#include "libruqolacore_export.h"
#include "messageattachmentfield.h"
#include <QDebug>
class MessageAttachmentField;
class LIBRUQOLACORE_EXPORT MessageAttachment
{
    Q_GADGET
    Q_PROPERTY(QString description READ description CONSTANT)
    Q_PROPERTY(QString color READ color CONSTANT)
    Q_PROPERTY(QString title READ title CONSTANT)
    Q_PROPERTY(QString link READ link CONSTANT)
    Q_PROPERTY(QString authorName READ authorName CONSTANT)
    Q_PROPERTY(int imageHeight READ imageHeight CONSTANT)
    Q_PROPERTY(int imageWidth READ imageWidth CONSTANT)
    Q_PROPERTY(bool canDownloadAttachment READ canDownloadAttachment CONSTANT)
    Q_PROPERTY(QString displayTitle READ displayTitle CONSTANT)
    Q_PROPERTY(QString imageTitle READ imageTitle CONSTANT)
    Q_PROPERTY(QString mimeType READ mimeType CONSTANT)
    Q_PROPERTY(bool isAnimatedImage READ isAnimatedImage CONSTANT)
public:
    MessageAttachment();

    enum AttachmentType { Unknown, NormalText, File, Video, Audio, Image };
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
    int mImageHeight = -1;
    int mImageWidth = -1;
    bool mIsAnimatedImage = false;
    bool mCollapsed = false;
    bool mShowAttachment = false;
};
Q_DECLARE_TYPEINFO(MessageAttachment, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const MessageAttachment &t);

#endif // MESSAGEATTACHMENT_H
