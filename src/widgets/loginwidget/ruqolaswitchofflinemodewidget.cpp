/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolaswitchofflinemodewidget.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

using namespace Qt::Literals::StringLiterals;
RuqolaSwitchOfflineModeWidget::RuqolaSwitchOfflineModeWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto label = new QLabel(i18n("Not network found."), this);
    label->setObjectName(u"label"_s);
    mainLayout->addWidget(label);

    auto button = new QPushButton(i18nc("@action:button", "Switch Offline Mode"), this);
    button->setObjectName(u"button"_s);
    connect(button, &QPushButton::clicked, this, &RuqolaSwitchOfflineModeWidget::switchOfflineMode);
}

RuqolaSwitchOfflineModeWidget::~RuqolaSwitchOfflineModeWidget() = default;

#include "moc_ruqolaswitchofflinemodewidget.cpp"
