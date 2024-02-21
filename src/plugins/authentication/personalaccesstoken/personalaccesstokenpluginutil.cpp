/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokenpluginutil.h"

bool PersonalAccessTokenPluginUtil::PersonalAccessTokenPluginInfo::isEmpty() const
{
    return userId.isEmpty() && token.isEmpty();
}

bool PersonalAccessTokenPluginUtil::PersonalAccessTokenPluginInfo::isValid() const
{
    return !isEmpty();
}
