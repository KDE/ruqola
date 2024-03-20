/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QMap>
class LIBRUQOLACORE_EXPORT MessageTranslation : public QSharedData
{
public:
    MessageTranslation();
    explicit MessageTranslation(const MessageTranslation &other);
    ~MessageTranslation();
    void parse(const QJsonObject &o);
    [[nodiscard]] bool operator==(const MessageTranslation &other) const;

    [[nodiscard]] QMap<QString, QString> translatedString() const;
    void setTranslatedString(const QMap<QString, QString> &translatedString);

    [[nodiscard]] QString translatedStringFromLanguage(const QString &lang) const;

    static QJsonArray serialize(const MessageTranslation &translation);
    static MessageTranslation *deserialize(const QJsonArray &o);

    [[nodiscard]] bool isEmpty() const;

private:
    QMap<QString, QString> mTranslatedString;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const MessageTranslation &t);
