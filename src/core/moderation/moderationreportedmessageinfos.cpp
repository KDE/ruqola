/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportedmessageinfos.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(ModerationReportedMessageInfos, Ruqola_ModerationReportedMessageInfos)

#include "ruqola_debug.h"

#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
ModerationReportedMessageInfos::ModerationReportedMessageInfos() = default;

QDebug operator<<(QDebug d, const ModerationReportedMessageInfos &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "ModerationInfosCount" << t.moderationInfosCount() << "\n";
    for (int i = 0, total = t.moderationInfosList().count(); i < total; ++i) {
        d.space() << t.moderationInfosList().at(i) << "\n";
    }
    return d;
}

int ModerationReportedMessageInfos::offset() const
{
    return mOffset;
}

void ModerationReportedMessageInfos::setOffset(int newOffset)
{
    mOffset = newOffset;
}

int ModerationReportedMessageInfos::total() const
{
    return mTotal;
}

void ModerationReportedMessageInfos::setTotal(int newTotal)
{
    mTotal = newTotal;
}

int ModerationReportedMessageInfos::moderationInfosCount() const
{
    return mModerationInfosCount;
}

void ModerationReportedMessageInfos::setModerationInfosCount(int newModerationInfosCount)
{
    mModerationInfosCount = newModerationInfosCount;
}

const QList<ModerationReportedMessageInfo> &ModerationReportedMessageInfos::moderationInfosList() const
{
    return mModerationInfosList;
}

void ModerationReportedMessageInfos::setModerationInfosList(const QList<ModerationReportedMessageInfo> &newModerationInfosList)
{
    mModerationInfosList = newModerationInfosList;
}

bool ModerationReportedMessageInfos::isEmpty() const
{
    return mModerationInfosList.isEmpty();
}

void ModerationReportedMessageInfos::clear()
{
    mModerationInfosList.clear();
}

int ModerationReportedMessageInfos::count() const
{
    return mModerationInfosList.count();
}

ModerationReportedMessageInfo ModerationReportedMessageInfos::at(int index) const
{
    if (index < 0 || index >= mModerationInfosList.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return {};
    }
    return mModerationInfosList.at(index);
}

void ModerationReportedMessageInfos::parseModerationInfos(const QJsonObject &moderationInfosObj)
{
    mModerationInfosList.clear();
    mModerationInfosCount = moderationInfosObj["count"_L1].toInt();
    mOffset = moderationInfosObj["offset"_L1].toInt();
    mTotal = moderationInfosObj["total"_L1].toInt();
    mModerationInfosList.reserve(mModerationInfosCount);
    parseModerationInfosObj(moderationInfosObj);
}

void ModerationReportedMessageInfos::parseModerationInfosObj(const QJsonObject &moderationInfosObj)
{
    const QJsonArray moderationsArray = moderationInfosObj["reports"_L1].toArray();
    for (const QJsonValue &current : moderationsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject moderationObject = current.toObject();
            ModerationReportedMessageInfo m;
            m.parseModerationInfo(moderationObject);
            mModerationInfosList.append(std::move(m));
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing moderations" << current;
        }
    }
}

void ModerationReportedMessageInfos::parseMoreModerationInfos(const QJsonObject &moderationInfosObj)
{
    const int moderationInfosCount = moderationInfosObj["count"_L1].toInt();
    mOffset = moderationInfosObj["offset"_L1].toInt();
    mTotal = moderationInfosObj["total"_L1].toInt();
    parseModerationInfosObj(moderationInfosObj);
    mModerationInfosCount += moderationInfosCount;
}

ModerationReportedMessageInfo ModerationReportedMessageInfos::takeAt(int index)
{
    return mModerationInfosList.takeAt(index);
}
