/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureservermcpwidget.h"
#include "ruqolaglobalconfig.h"
#include <KLocalizedString>
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
    RuqolaGlobalConfig::self()->setEnableMcpServer(mEnabled->isChecked());
}

void ConfigureServerMcpWidget::load()
{
    mEnabled->setChecked(RuqolaGlobalConfig::self()->enableMcpServer());
}

void ConfigureServerMcpWidget::restoreToDefaults()
{
    // TODO
}
