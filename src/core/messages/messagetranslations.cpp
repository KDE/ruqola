/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "messagetranslations.h"

#include "ruqola_message_memory_debug.h"
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
MessageTranslations::MessageTranslations()

{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " MessageTranslation created " << this;
}

MessageTranslations::MessageTranslations(const MessageTranslations &other)
    : QSharedData(other)
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " MessageTranslation created " << this;
    mTranslatedString = other.mTranslatedString;
}

MessageTranslations::~MessageTranslations()
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " MessageTranslation deleted " << this;
}

QDebug operator<<(QDebug d, const MessageTranslations &t)
{
    d.space() << "translate string" << t.translatedString();
    return d;
}

void MessageTranslations::parse(const QJsonObject &obj)
{
    mTranslatedString.clear();
    const QJsonObject languageObject = obj["translations"_L1].toObject();
    const QStringList keys = languageObject.keys();
    for (const QString &lang : keys) {
        mTranslatedString.insert(lang, languageObject.value(lang).toString());
    }
    // qDebug() << " void MessageTranslation::parse(const QJsonObject &obj)"<<mTranslatedString;
}

bool MessageTranslations::operator==(const MessageTranslations &other) const
{
    return mTranslatedString == other.translatedString();
}

QMap<QString, QString> MessageTranslations::translatedString() const
{
    return mTranslatedString;
}

void MessageTranslations::setTranslatedString(const QMap<QString, QString> &translatedString)
{
    mTranslatedString = translatedString;
}

QString MessageTranslations::translatedStringFromLanguage(const QString &lang) const
{
    return mTranslatedString.value(lang);
}

QJsonArray MessageTranslations::serialize(const MessageTranslations &translation)
{
    QJsonArray array;
    for (const auto &[key, value] : translation.mTranslatedString.asKeyValueRange()) {
        QJsonObject obj;
        obj.insert(key, value);
        array.append(obj);
    }
    return array;
}

MessageTranslations *MessageTranslations::deserialize(const QJsonArray &array)
{
    MessageTranslations *translationMessage = new MessageTranslations;
    QMap<QString, QString> translationStrings;
    for (int i = 0, total = array.count(); i < total; ++i) {
        const QJsonObject o = array.at(i).toObject();
        if (o.count() == 1) {
            translationStrings.insert(o.keys().at(0), o.value(o.keys().at(0)).toString());
        }
    }
    translationMessage->setTranslatedString(translationStrings);
    return translationMessage;
}

bool MessageTranslations::isEmpty() const
{
    return mTranslatedString.isEmpty();
}
