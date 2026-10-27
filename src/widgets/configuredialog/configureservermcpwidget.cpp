/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureservermcpwidget.h"
#include "ruqolaglobalconfig.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QCheckBox>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolWidgets/McpProtocolServerConfigureWidget>

using namespace Qt::Literals::StringLiterals;
ConfigureServerMcpWidget::ConfigureServerMcpWidget(QWidget *parent)
    : QWidget{parent}
    , mMcpProtocolServerConfigureWidget(new TextAutoGenerateTextMcpProtocolWidgets::McpProtocolServerConfigureWidget(this))
    , mEnabled(new QCheckBox(i18nc("@option:check", "Enable")))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mEnabled->setObjectName(u"mEnabled"_s);
    mainLayout->addWidget(mEnabled);

    mMcpProtocolServerConfigureWidget->setObjectName(u"mMcpProtocolServerConfigureWidget"_s);
    mainLayout->addWidget(mMcpProtocolServerConfigureWidget);

    connect(mEnabled,
            &QCheckBox::toggled,
            mMcpProtocolServerConfigureWidget,
            &TextAutoGenerateTextMcpProtocolWidgets::McpProtocolServerConfigureWidget::setEnabled);
    mMcpProtocolServerConfigureWidget->setEnabled(false);
}

ConfigureServerMcpWidget::~ConfigureServerMcpWidget() = default;

void ConfigureServerMcpWidget::save()
{
    const TextAutoGenerateTextMcpProtocolCore::McpProtocolSettings settings = mMcpProtocolServerConfigureWidget->settings();
    KConfigGroup group(KSharedConfig::openConfig(), u"mcpserver"_s);
    settings.save(group);
    RuqolaGlobalConfig::self()->setEnableMcpServer(mEnabled->isChecked());
    RuqolaGlobalConfig::self()->save();
}

void ConfigureServerMcpWidget::load()
{
    mEnabled->setChecked(RuqolaGlobalConfig::self()->enableMcpServer());
    TextAutoGenerateTextMcpProtocolCore::McpProtocolSettings settings;
    const KConfigGroup group(KSharedConfig::openConfig(), u"mcpserver"_s);
    settings.load(group);
    mMcpProtocolServerConfigureWidget->setSettings(settings);
}

void ConfigureServerMcpWidget::restoreToDefaults()
{
    const bool bUseDefaults = RuqolaGlobalConfig::self()->useDefaults(true);
    mEnabled->setChecked(RuqolaGlobalConfig::self()->enableMcpServer());
    RuqolaGlobalConfig::self()->useDefaults(bUseDefaults);
    mMcpProtocolServerConfigureWidget->setSettings(TextAutoGenerateTextMcpProtocolCore::McpProtocolSettings{});
}

#include "moc_configureservermcpwidget.cpp"
