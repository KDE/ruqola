/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "welcomewidget.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QPushButton>

using namespace Qt::Literals::StringLiterals;
WelcomeWidget::WelcomeWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mainLayout->addStretch(1);

    auto addAccountButton = new QPushButton(i18nc("@action:button", "Add Account…"), this);
    addAccountButton->setObjectName(u"addAccountButton"_s);
    addAccountButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(addAccountButton, &QPushButton::clicked, this, &WelcomeWidget::createNewAccount);
    mainLayout->addWidget(addAccountButton);

    mainLayout->addStretch(1);
}

WelcomeWidget::~WelcomeWidget() = default;

#include "moc_welcomewidget.cpp"
