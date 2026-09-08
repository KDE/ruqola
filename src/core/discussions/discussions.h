/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "discussion.h"
#include "libruqola_private_export.h"
#include "paginatedinfolist.h"
class QDebug;

class LIBRUQOLACORE_TESTS_EXPORT Discussions : public PaginatedInfoList<Discussion, &Discussion::parseDiscussion>
{
public:
    void parseDiscussions(const QJsonObject &obj);
    void parseMoreDiscussions(const QJsonObject &obj);

    void append(const Discussion &discussion);
};
QT_DECL_METATYPE_EXTERN_TAGGED(Discussions, Ruqola_Discussions, LIBRUQOLACORE_EXPORT)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Discussions &t);
