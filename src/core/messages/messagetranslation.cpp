/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "messagetranslation.h"
#include <QJsonObject>

MessageTranslation::MessageTranslation() = default;

QDebug operator<<(QDebug d, const MessageTranslation &t)
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
    // qDebug() << " void MessageTranslation::parse(const QJsonObject &obj)"<<mTranslatedString;
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

QJsonObject MessageTranslation::serialize(const MessageTranslation &url)
{
    QJsonObject obj;
    // TODO
    return obj;
}

MessageTranslation MessageTranslation::deserialize(const QJsonObject &o)
{
    MessageTranslation translationMessage;
    // TODO
    return translationMessage;
}
