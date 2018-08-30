/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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

#ifndef MESSAGEURL_H
#define MESSAGEURL_H

#include "libruqola_private_export.h"
#include <QDebug>
class LIBRUQOLACORE_TESTS_EXPORT MessageUrl
{
    Q_GADGET
    Q_PROPERTY(QString url READ url WRITE setUrl FINAL)
    Q_PROPERTY(QString pageTitle READ pageTitle WRITE setPageTitle FINAL)
    Q_PROPERTY(QString description READ description WRITE setDescription FINAL)
public:
    MessageUrl();
    Q_REQUIRED_RESULT bool operator==(const MessageUrl &other) const;
    static QJsonObject serialize(const MessageUrl &url);
    static MessageUrl fromJSon(const QJsonObject &o);

    Q_REQUIRED_RESULT QString url() const;
    void setUrl(const QString &url);

    Q_REQUIRED_RESULT QString pageTitle() const;
    void setPageTitle(const QString &pageTitle);

    Q_REQUIRED_RESULT bool isEmpty() const;

    Q_REQUIRED_RESULT QString description() const;
    void setDescription(const QString &description);

private:
    QString mUrl;
    QString mPageTitle;
    QString mDescription;
};
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const MessageUrl &t);
Q_DECLARE_TYPEINFO(MessageUrl, Q_MOVABLE_TYPE);

#endif // MESSAGEURL_H
