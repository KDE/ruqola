/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "messagetranslation.h"
#include <QJsonArray>
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

QJsonArray MessageTranslation::serialize(const MessageTranslation &translation)
{
    QJsonArray array;
    QMapIterator<QString, QString> i(translation.mTranslatedString);
    while (i.hasNext()) {
        i.next();
        QJsonObject obj;
        obj.insert(i.key(), i.value());
        array.append(obj);
    }
    return array;
}

MessageTranslation MessageTranslation::deserialize(const QJsonArray &array)
{
    MessageTranslation translationMessage;
    QMap<QString, QString> translationStrings;
    for (int i = 0, total = array.count(); i < total; ++i) {
        QJsonObject o = array.at(i).toObject();
        if (o.count() == 1) {
            translationStrings.insert(o.keys().at(0), o.value(o.keys().at(0)).toString());
        }
    }
    translationMessage.setTranslatedString(translationStrings);
    return translationMessage;
}

bool MessageTranslation::isEmpty() const
{
    return mTranslatedString.isEmpty();
}
