/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

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

#include <QJsonObject>

MessageAttachment::MessageAttachment()
{
}

QJsonObject MessageAttachment::serialize(const MessageAttachment &message)
{
    QJsonObject obj;
    obj[QStringLiteral("description")]  = message.description();
    obj[QStringLiteral("title")]  = message.title();
    obj[QStringLiteral("url")]  = message.link();
    return obj;
}

MessageAttachment MessageAttachment::fromJSon(const QJsonObject &o)
{
    MessageAttachment att;
    att.setDescription(o.value(QStringLiteral("description")).toString());
    att.setTitle(o.value(QStringLiteral("title")).toString());
    att.setLink(o.value(QStringLiteral("url")).toString());
    return att;
}

bool MessageAttachment::isEmpty() const
{
    return mLink.isEmpty();
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
    return (mDescription == other.description()) && (mTitle == other.title()) && (mLink == other.link());
}

QDebug operator <<(QDebug d, const MessageAttachment &t)
{
    d << "Title : " << t.title();
    d << "Description: " << t.description();
    d << "Link: " << t.link();
    return d;
}
