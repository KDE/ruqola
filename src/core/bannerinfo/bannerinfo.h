/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>

#include <QString>

class LIBRUQOLACORE_EXPORT BannerInfo
{
public:
    BannerInfo();
    ~BannerInfo();

    [[nodiscard]] bool isValid() const;

    void parseBannerInfo(const QJsonObject &object);

    [[nodiscard]] bool operator==(const BannerInfo &other) const;

    [[nodiscard]] const QString &text() const;
    void setText(const QString &newText);

    [[nodiscard]] const QString &title() const;
    void setTitle(const QString &newTitle);

    [[nodiscard]] const QString &link() const;
    void setLink(const QString &newLink);

    [[nodiscard]] bool read() const;
    void setRead(bool newRead);

    [[nodiscard]] const QStringList &textArguments() const;
    void setTextArguments(const QStringList &newTextArguments);

    [[nodiscard]] const QString &identifier() const;
    void setIdentifier(const QString &newIdentifier);

    [[nodiscard]] int priority() const;
    void setPriority(int newPriority);

    [[nodiscard]] static QString defaultText(const BannerInfo &info);

private:
    QStringList mTextArguments;
    QString mText;
    QString mTitle;
    QString mLink;
    QString mIdentifier;
    int mPriority = -1;
    bool mRead = false;
};

Q_DECLARE_METATYPE(BannerInfo)
Q_DECLARE_TYPEINFO(BannerInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const BannerInfo &t);
