/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "config-ruqola.h"
#include "libruqolawidgets_private_export.h"
#include <KPluginMetaData>
#include <QList>
#include <QObject>
#include <TextAddonsWidgets/PluginUtil>
class PluginText;

class TextPluginManagerInfo
{
public:
    TextPluginManagerInfo() = default;

    KPluginMetaData data;
    QString metaDataFileNameBaseName;
    QString metaDataFileName;
    TextAddonsWidgets::PluginUtilData pluginData;
    PluginText *plugin = nullptr;
    bool isEnabled = true;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT TextPluginManager : public QObject
{
    Q_OBJECT
public:
    ~TextPluginManager() override;
    static TextPluginManager *self();

    [[nodiscard]] QList<PluginText *> pluginsList() const;

    [[nodiscard]] QString configGroupName() const;
    [[nodiscard]] QString configPrefixSettingKey() const;

    [[nodiscard]] QList<TextAddonsWidgets::PluginUtilData> pluginDataList() const;

    [[nodiscard]] PluginText *pluginFromIdentifier(const QString &identifier) const;

private:
    explicit TextPluginManager(QObject *parent = nullptr);
    LIBRUQOLAWIDGETS_NO_EXPORT void initializePluginList();
    LIBRUQOLAWIDGETS_NO_EXPORT void loadPlugin(TextPluginManagerInfo *item);
    QList<TextPluginManagerInfo> mPluginList;
    QList<TextAddonsWidgets::PluginUtilData> mPluginDataList;
};
