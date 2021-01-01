/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef TEXTPLUGINMANAGER_H
#define TEXTPLUGINMANAGER_H

#include <KPluginMetaData>
#include <QObject>
#include <QVector>
#include "libruqolawidgets_private_export.h"

class PluginText;

class PluginTextUtilData
{
public:
    PluginTextUtilData()
    {
    }

    QStringList mExtraInfo;
    QString mIdentifier;
    QString mName;
};

class TextPluginManagerInfo
{
public:
    TextPluginManagerInfo()
    {
    }

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
    bool initializePluginList();
    void loadPlugin(TextPluginManagerInfo *item);
    PluginTextUtilData createPluginMetaData(const KPluginMetaData &metaData);
    QVector<TextPluginManagerInfo> mPluginList;
    QVector<PluginTextUtilData> mPluginDataList;
};

#endif // TEXTPLUGINMANAGER_H
