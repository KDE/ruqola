/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include <KPluginMetaData>
#include <QString>

namespace PluginUtils
{
struct PluginUtilData {
    QString mIdentifier;
    QString mName;
    bool mIsEnabled = false;
};

[[nodiscard]] PluginUtils::PluginUtilData createPluginMetaData(const KPluginMetaData &metaData);
};
