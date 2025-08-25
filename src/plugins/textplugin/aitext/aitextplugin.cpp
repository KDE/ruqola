/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "aitextplugin.h"
#include "aitextinterface.h"
#include <KPluginFactory>
#include <TextAutoGenerateText/TextAutoGenerateMenuConfigureDialog>
#include <TextAutoGenerateText/TextAutoGenerateMenuTextManager>
K_PLUGIN_CLASS_WITH_JSON(AiTextPlugin, "ruqola_aitextplugin.json")

AiTextPlugin::AiTextPlugin(QObject *parent, const QVariantList &)
    : PluginText(parent)
    , mManager(new TextAutoGenerateText::TextAutoGenerateMenuTextManager(this))
{
}

AiTextPlugin::~AiTextPlugin() = default;

PluginTextInterface *AiTextPlugin::createInterface(QObject *parent)
{
    return new AiTextInterface(mManager, parent);
}

int AiTextPlugin::order() const
{
    return 30;
}

bool AiTextPlugin::hasConfigureDialog() const
{
    return true;
}

void AiTextPlugin::showConfigureDialog(QWidget *parent) const
{
    auto dlg = new TextAutoGenerateText::TextAutoGenerateMenuConfigureDialog(nullptr);
    dlg->setTextInfos(mManager->textInfos());
    if (dlg->exec()) {
        mManager->setTextInfos(dlg->textInfos());
        mManager->save();
    }
    delete dlg;
}

#include "aitextplugin.moc"

#include "moc_aitextplugin.cpp"
