/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratormoderationrangewidget.h"
#include <KLocalizedString>
#include <QAction>
#include <QDateEdit>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QToolButton>

AdministratorModerationRangeWidget::AdministratorModerationRangeWidget(QWidget *parent)
    : QWidget{parent}
    , mFromDate(new QDateEdit(this))
    , mToDate(new QDateEdit(this))
    , mFilterDate(new QToolButton(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mFromDate->setObjectName(QStringLiteral("mFromDate"));
    mToDate->setObjectName(QStringLiteral("mToDate"));

    mFilterDate->setObjectName(QStringLiteral("mFilterDate"));
    mFilterDate->setAutoRaise(true);
    mFilterDate->setPopupMode(QToolButton::InstantPopup);
    mFilterDate->setIcon(QIcon::fromTheme(QStringLiteral("application-menu")));
    // connect(mChannelActionPopupMenu, &ChannelActionPopupMenu::actionRequested, this, &RoomHeaderWidget::actionRequested);

    auto fromLabel = new QLabel(i18n("From:"), this);
    fromLabel->setObjectName(QStringLiteral("fromLabel"));

    auto toLabel = new QLabel(i18n("To:"), this);
    fromLabel->setObjectName(QStringLiteral("toLabel"));

    mainLayout->addWidget(fromLabel);
    mainLayout->addWidget(mFromDate);
    mainLayout->addWidget(toLabel);
    mainLayout->addWidget(mToDate);
    mainLayout->addWidget(mFilterDate);
    mainLayout->addStretch();
}

AdministratorModerationRangeWidget::~AdministratorModerationRangeWidget() = default;

void AdministratorModerationRangeWidget::initializeMenu()
{
    auto *menu = new QMenu(mFilterDate);
    auto todayAction = new QAction(i18n("Today"), menu);
    menu->addAction(todayAction);
    connect(todayAction, &QAction::triggered, this, [this]() {
        // TODO
    });
    mFilterDate->setMenu(menu);
    // TODO
}

#include "moc_administratormoderationrangewidget.cpp"
