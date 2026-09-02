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

QList<ReadReceipt> ReadReceipts::readReceipts() const
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
    if (index < 0 || index >= mReadReceipts.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return;
    }
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

void ReadReceipts::parseReadReceipts(const QJsonArray &array)
{
    mReadReceipts.clear();
    const auto readReceiptsArrayCount = array.count();
    mReadReceipts.reserve(readReceiptsArrayCount);
    for (auto i = 0; i < readReceiptsArrayCount; ++i) {
        ReadReceipt r;
        const auto obj = array.at(i).toObject();
        r.parseReadReceiptInfo(obj);
        if (r.isValid()) {
            mReadReceipts.append(std::move(r));
        } else {
            qCWarning(RUQOLA_LOG) << "Invalid Read Receipt Info: " << obj;
        }
    }
}

QDebug operator<<(QDebug d, const ReadReceipts &t)
{
    const auto &list = t.readReceipts();
    for (int i = 0, total = list.count(); i < total; ++i) {
        d.space() << list.at(i) << "\n";
    }
    return d;
}

bool ReadReceipts::operator==(const ReadReceipts &other) const
{
    return mReadReceipts == other.mReadReceipts;
}
