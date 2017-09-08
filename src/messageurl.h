/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

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
#include <QObject>
#include <QDebug>
class LIBRUQOLACORE_TESTS_EXPORT MessageUrl
{
public:
    MessageUrl();
    bool operator==(const MessageUrl &other) const;
    static QJsonObject serialize(const MessageUrl &url);
    static MessageUrl fromJSon(const QJsonObject &o);

    QString url() const;
    void setUrl(const QString &url);

    QString pageTitle() const;
    void setPageTitle(const QString &pageTitle);

    bool isEmpty() const;

    QString description() const;
    void setDescription(const QString &description);

private:
    //TODO add more
    QString mUrl;
    QString mPageTitle;
    QString mDescription;
};
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const MessageUrl &t);
Q_DECLARE_TYPEINFO(MessageUrl, Q_MOVABLE_TYPE);

#endif // MESSAGEURL_H
