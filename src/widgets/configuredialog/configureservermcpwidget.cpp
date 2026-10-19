/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureservermcpwidget.h"
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolWidgets/McpProtocolServerConfigureWidget>

using namespace Qt::Literals::StringLiterals;
ConfigureServerMcpWidget::ConfigureServerMcpWidget(QWidget *parent)
    : QWidget{parent}
    , mMcpProtocolServerConfigureWidget(new TextAutoGenerateTextMcpProtocolWidgets::McpProtocolServerConfigureWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mMcpProtocolServerConfigureWidget->setObjectName(u"mMcpProtocolServerConfigureWidget"_s);
    mainLayout->addWidget(mMcpProtocolServerConfigureWidget);
}

ConfigureServerMcpWidget::~ConfigureServerMcpWidget() = default;

void ConfigureServerMcpWidget::save()
{
    // TODO
}

void ConfigureServerMcpWidget::load()
{
    // TODO
}

void ConfigureServerMcpWidget::restoreToDefaults()
{
    // TODO
}
