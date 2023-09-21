/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QDebug>
class LIBRUQOLACORE_TESTS_EXPORT MessageUrl
{
    Q_GADGET
public:
    MessageUrl();
    [[nodiscard]] bool operator==(const MessageUrl &other) const;
    static QJsonObject serialize(const MessageUrl &url);
    static MessageUrl deserialize(const QJsonObject &o);

    [[nodiscard]] QString url() const;
    void setUrl(const QString &url);

    [[nodiscard]] QString pageTitle() const;
    void setPageTitle(const QString &pageTitle);

    [[nodiscard]] bool isEmpty() const;

    [[nodiscard]] QString description() const;
    void setDescription(const QString &description);

private:
    QString mUrl;
    QString mPageTitle;
    QString mDescription;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const MessageUrl &t);
Q_DECLARE_TYPEINFO(MessageUrl, Q_MOVABLE_TYPE);
