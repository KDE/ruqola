/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>
#include <QString>

class LIBRUQOLACORE_EXPORT BannerInfo
{
public:
    BannerInfo();
    ~BannerInfo();

    Q_REQUIRED_RESULT bool isValid() const;

    void parseBannerInfo(const QJsonObject &object);

    Q_REQUIRED_RESULT bool operator==(const BannerInfo &other) const;

    Q_REQUIRED_RESULT const QString &text() const;
    void setText(const QString &newText);

    Q_REQUIRED_RESULT const QString &title() const;
    void setTitle(const QString &newTitle);

    Q_REQUIRED_RESULT const QString &link() const;
    void setLink(const QString &newLink);

    Q_REQUIRED_RESULT bool read() const;
    void setRead(bool newRead);

    Q_REQUIRED_RESULT const QStringList &textArguments() const;
    void setTextArguments(const QStringList &newTextArguments);

    Q_REQUIRED_RESULT const QString &identifier() const;
    void setIdentifier(const QString &newIdentifier);

    Q_REQUIRED_RESULT int priority() const;
    void setPriority(int newPriority);

    Q_REQUIRED_RESULT static QString defaultText(const BannerInfo &info);

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
Q_DECLARE_TYPEINFO(BannerInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const BannerInfo &t);
