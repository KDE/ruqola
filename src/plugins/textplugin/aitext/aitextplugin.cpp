/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "aitextplugin.h"
#include "aitextinterface.h"
#include "textautogeneratetext_version.h"
#include <KPluginFactory>
#if TEXTAUTOGENERATETEXT_VERSION >= QT_VERSION_CHECK(1, 6, 1)
#include <TextAutoGenerateText/TextAutoGenerateMenuConfigureDialog>
#include <TextAutoGenerateText/TextAutoGenerateMenuTextManager>
#endif
K_PLUGIN_CLASS_WITH_JSON(AiTextPlugin, "ruqola_aitextplugin.json")

AiTextPlugin::AiTextPlugin(QObject *parent, const QVariantList &)
    : PluginText(parent)
{
#if TEXTAUTOGENERATETEXT_VERSION >= QT_VERSION_CHECK(1, 6, 1)
    mManager = new TextAutoGenerateText::TextAutoGenerateMenuTextManager(this);
#endif
}

AiTextPlugin::~AiTextPlugin() = default;

PluginTextInterface *AiTextPlugin::createInterface(QObject *parent)
{
#if TEXTAUTOGENERATETEXT_VERSION >= QT_VERSION_CHECK(1, 6, 1)
    return new AiTextInterface(mManager, parent);
#else
    return new AiTextInterface(parent);
#endif
}

int AiTextPlugin::order() const
{
    return 30;
}

bool AiTextPlugin::hasConfigureDialog() const
{
#if TEXTAUTOGENERATETEXT_VERSION >= QT_VERSION_CHECK(1, 6, 1)
    return true;
#else
    return false;
#endif
}

void AiTextPlugin::showConfigureDialog(QWidget *parent) const
{
#if TEXTAUTOGENERATETEXT_VERSION >= QT_VERSION_CHECK(1, 6, 1)
    auto dlg = new TextAutoGenerateText::TextAutoGenerateMenuConfigureDialog(nullptr);
    dlg->setTextInfos(mManager->textInfos());
    if (dlg->exec()) {
        mManager->setTextInfos(dlg->textInfos());
        mManager->save();
    }
    delete dlg;
#endif
}

#include "aitextplugin.moc"

#include "moc_aitextplugin.cpp"
