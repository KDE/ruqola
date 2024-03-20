/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "messagetranslation.h"
#include "ruqola_message_memory_debug.h"
#include <QJsonArray>
#include <QJsonObject>

MessageTranslation::MessageTranslation()
    : QSharedData()
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " MessageTranslation created " << this;
}

MessageTranslation::MessageTranslation(const MessageTranslation &other)
    : QSharedData(other)
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " MessageTranslation created " << this;
    mTranslatedString = other.mTranslatedString;
}

MessageTranslation::~MessageTranslation()
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " ModerationMessage deleted " << this;
}

QDebug operator<<(QDebug d, const MessageTranslation &t)
{
    d.space() << "translate string" << t.translatedString();
    return d;
}

void MessageTranslation::parse(const QJsonObject &obj)
{
    mTranslatedString.clear();
    const QJsonObject languageObject = obj[QLatin1StringView("translations")].toObject();
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

QString MessageTranslation::translatedStringFromLanguage(const QString &lang) const
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

MessageTranslation *MessageTranslation::deserialize(const QJsonArray &array)
{
    MessageTranslation *translationMessage = new MessageTranslation;
    QMap<QString, QString> translationStrings;
    for (int i = 0, total = array.count(); i < total; ++i) {
        QJsonObject o = array.at(i).toObject();
        if (o.count() == 1) {
            translationStrings.insert(o.keys().at(0), o.value(o.keys().at(0)).toString());
        }
    }
    translationMessage->setTranslatedString(translationStrings);
    return translationMessage;
}

bool MessageTranslation::isEmpty() const
{
    return mTranslatedString.isEmpty();
}
