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

#ifndef MESSAGEATTACHMENT_H
#define MESSAGEATTACHMENT_H

#include "libruqola_private_export.h"
#include <QDebug>
class LIBRUQOLACORE_TESTS_EXPORT MessageAttachment
{
    Q_GADGET
    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(QString color READ color WRITE setColor)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString link READ link WRITE setLink)
    Q_PROPERTY(int imageHeight READ imageHeight WRITE setImageHeight)
    Q_PROPERTY(int imageWidth READ imageWidth WRITE setImageWidth)
public:
    MessageAttachment();
    bool isEmpty() const;
    QString description() const;
    void setDescription(const QString &description);

    QString title() const;
    void setTitle(const QString &title);

    QString link() const;
    void setLink(const QString &link);

    bool operator==(const MessageAttachment &other) const;

    static QJsonObject serialize(const MessageAttachment &message);
    static MessageAttachment fromJSon(const QJsonObject &o);
    int imageWidth() const;
    void setImageWidth(int imageWidth);

    int imageHeight() const;
    void setImageHeight(int imageHeight);

    QString color() const;
    void setColor(const QString &color);

private:
    //TODO add fields support ?
    QString mColor;
    QString mDescription;
    QString mTitle;
    QString mLink;
    int mImageHeight = -1;
    int mImageWidth = -1;
};
Q_DECLARE_TYPEINFO(MessageAttachment, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const MessageAttachment &t);

#endif // MESSAGEATTACHMENT_H
