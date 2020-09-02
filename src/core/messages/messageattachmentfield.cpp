/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "messageattachmentfield.h"

#include <QJsonObject>

MessageAttachmentField::MessageAttachmentField()
{

}

MessageAttachmentField::~MessageAttachmentField()
{

}

bool MessageAttachmentField::operator==(const MessageAttachmentField &other) const
{
    return mTitle == other.title() &&
            mValue == other.value();
}

QString MessageAttachmentField::title() const
{
    return mTitle;
}

void MessageAttachmentField::setTitle(const QString &title)
{
    mTitle = title;
}

QString MessageAttachmentField::value() const
{
    return mValue;
}

void MessageAttachmentField::setValue(const QString &value)
{
    mValue = value;
}

QJsonObject MessageAttachmentField::serialize(const MessageAttachmentField &message)
{
    QJsonObject obj;
    obj[QStringLiteral("title")] = message.title();
    obj[QStringLiteral("value")] = message.value();
    return obj;
}

MessageAttachmentField MessageAttachmentField::fromJson(const QJsonObject &o)
{
    MessageAttachmentField att;
    att.setValue(o.value(QLatin1String("value")).toString());
    att.setTitle(o.value(QLatin1String("title")).toString());
    return att;
}

QDebug operator <<(QDebug d, const MessageAttachmentField &t)
{
    d << " title " << t.title();
    d << " value " << t.value();
    return d;
}
