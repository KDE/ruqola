/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <KPluginMetaData>
#include <QList>
#include <QObject>

class PluginText;

class PluginTextUtilData
{
public:
    PluginTextUtilData() = default;

    QStringList mExtraInfo;
    QString mIdentifier;
    QString mName;
};

class TextPluginManagerInfo
{
public:
    TextPluginManagerInfo() = default;

    KPluginMetaData data;
    QString metaDataFileNameBaseName;
    QString metaDataFileName;
    PluginTextUtilData pluginData;
    PluginText *plugin = nullptr;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT TextPluginManager : public QObject
{
    Q_OBJECT
public:
    ~TextPluginManager() override;
    static TextPluginManager *self();

    [[nodiscard]] QList<PluginText *> pluginsList() const;

private:
    explicit TextPluginManager(QObject *parent = nullptr);
    LIBRUQOLAWIDGETS_NO_EXPORT void initializePluginList();
    LIBRUQOLAWIDGETS_NO_EXPORT void loadPlugin(TextPluginManagerInfo *item);
    LIBRUQOLAWIDGETS_NO_EXPORT PluginTextUtilData createPluginMetaData(const KPluginMetaData &metaData);
    QList<TextPluginManagerInfo> mPluginList;
    QList<PluginTextUtilData> mPluginDataList;
};
