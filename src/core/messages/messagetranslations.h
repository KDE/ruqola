/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QJsonArray>
#include <QMap>
class LIBRUQOLACORE_EXPORT MessageTranslations : public QSharedData
{
public:
    MessageTranslations();
    explicit MessageTranslations(const MessageTranslations &other);
    ~MessageTranslations();
    void parse(const QJsonObject &o);
    [[nodiscard]] bool operator==(const MessageTranslations &other) const;

    [[nodiscard]] QMap<QString, QString> translatedString() const;
    void setTranslatedString(const QMap<QString, QString> &translatedString);

    [[nodiscard]] QString translatedStringFromLanguage(const QString &lang) const;

    [[nodiscard]] static QJsonArray serialize(const MessageTranslations &translation);
    [[nodiscard]] static MessageTranslations *deserialize(const QJsonArray &o);

    [[nodiscard]] bool isEmpty() const;

private:
    QMap<QString, QString> mTranslatedString;
};
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const MessageTranslations &t);
