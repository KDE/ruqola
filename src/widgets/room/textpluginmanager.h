/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <KPluginMetaData>
#include <QObject>
#include <QVector>

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
    explicit TextPluginManager(QObject *parent = nullptr);
    ~TextPluginManager() override;
    static TextPluginManager *self();

    Q_REQUIRED_RESULT QVector<PluginText *> pluginsList() const;

private:
    Q_DISABLE_COPY(TextPluginManager)
    void initializePluginList();
    void loadPlugin(TextPluginManagerInfo *item);
    PluginTextUtilData createPluginMetaData(const KPluginMetaData &metaData);
    QVector<TextPluginManagerInfo> mPluginList;
    QVector<PluginTextUtilData> mPluginDataList;
};

