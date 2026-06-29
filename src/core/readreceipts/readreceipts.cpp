/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "readreceipts.h"

#include "ruqola_debug.h"

#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
ReadReceipts::ReadReceipts() = default;

QList<ReadReceipt> ReadReceipts::readReceopts() const
{
    return mReadReceipts;
}

int ReadReceipts::count() const
{
    return mReadReceipts.count();
}

void ReadReceipts::clear()
{
    mReadReceipts.clear();
}

ReadReceipt ReadReceipts::at(int index) const
{
    if (index < 0 || index >= mReadReceipts.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return {};
    }
    return mReadReceipts.at(index);
}

void ReadReceipts::removeAt(int index)
{
    mReadReceipts.removeAt(index);
}

bool ReadReceipts::isEmpty() const
{
    return mReadReceipts.isEmpty();
}

void ReadReceipts::setReadReceipts(const QList<ReadReceipt> &readReceptsInfos)
{
    mReadReceipts = readReceptsInfos;
}

void ReadReceipts::parseReadReceipts(const QJsonObject &obj)
{
    mReadReceipts.clear();
#if 0
    const QJsonArray tokensArray = obj["tokens"_L1].toArray();
    const auto tokensArrayCount = tokensArray.count();
    mReadReceipts.reserve(tokensArrayCount);
    for (auto i = 0; i < tokensArrayCount; ++i) {
        PersonalAccessTokenInfo r;
        r.parsePersonalAccessTokenInfo(tokensArray.at(i).toObject());
        if (r.isValid()) {
            mReadReceipts.append(std::move(r));
        } else {
            qCWarning(RUQOLA_LOG) << "Invalid personal Access Token Info: " << tokensArray.at(i).toObject();
        }
    }
#endif
}

QDebug operator<<(QDebug d, const ReadReceipts &t)
{
    for (int i = 0, total = t.readReceopts().count(); i < total; ++i) {
        d.space() << t.readReceopts().at(i) << "\n";
    }
    return d;
}

bool ReadReceipts::operator==(const ReadReceipts &other) const
{
    return readReceopts() == other.readReceopts();
}
