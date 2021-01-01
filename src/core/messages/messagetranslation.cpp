/*
   Copyright (c) 2019-2021 Laurent Montel <montel@kde.org>

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
#include "messagetranslation.h"
#include <QJsonObject>

MessageTranslation::MessageTranslation()
{
}

QDebug operator <<(QDebug d, const MessageTranslation &t)
{
    d << " translate string " << t.translatedString();
    return d;
}

void MessageTranslation::parse(const QJsonObject &obj)
{
    mTranslatedString.clear();
    const QJsonObject languageObject = obj[QLatin1String("translations")].toObject();
    const QStringList keys = languageObject.keys();
    for (const QString &lang : keys) {
        mTranslatedString.insert(lang, languageObject.value(lang).toString());
    }
    //qDebug() << " void MessageTranslation::parse(const QJsonObject &obj)"<<mTranslatedString;
}

bool MessageTranslation::operator==(const MessageTranslation &other) const
{
    return mTranslatedString == other.translatedString();
}

QMap<QString, QString> MessageTranslation::translatedString() const
{
    return mTranslatedString;
}

void MessageTranslation::setTranslatedString(const QMap<QString, QString> &translatedString)
{
    mTranslatedString = translatedString;
}

QString MessageTranslation::translatedStringFromLanguage(const QString &lang)
{
    return mTranslatedString.value(lang);
}
