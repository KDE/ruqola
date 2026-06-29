/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include "readreceipt.h"
#include <QList>
class QDebug;
class LIBRUQOLACORE_EXPORT ReadReceipts
{
public:
    ReadReceipts();

    [[nodiscard]] QList<ReadReceipt> readReceopts() const;
    void setReadReceipts(const QList<ReadReceipt> &tokenInfos);

    void parseReadReceipts(const QJsonObject &obj);
    [[nodiscard]] bool isEmpty() const;

    [[nodiscard]] int count() const;
    void clear();

    [[nodiscard]] ReadReceipt at(int index) const;

    void removeAt(int index);

    [[nodiscard]] bool operator==(const ReadReceipts &other) const;

private:
    QList<ReadReceipt> mReadReceipts;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ReadReceipts &t);
