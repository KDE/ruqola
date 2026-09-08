/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "discussions/discussions.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(Discussions, Ruqola_Discussions)

using namespace Qt::Literals::StringLiterals;

void Discussions::parseDiscussions(const QJsonObject &obj)
{
    parseInfos(obj, "discussions"_L1);
}

void Discussions::parseMoreDiscussions(const QJsonObject &obj)
{
    parseMoreInfos(obj, "discussions"_L1);
}

void Discussions::append(const Discussion &discussion)
{
    mList.append(discussion);
}

QDebug operator<<(QDebug d, const Discussions &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "discussionsCount" << t.loadedCount() << "\n";
    for (const Discussion &discussion : t.list()) {
        d.space() << discussion << "\n";
    }
    return d;
}
