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
    initializeMenu();
    // TODO verify that range is valid.
}

AdministratorModerationRangeWidget::DateRange AdministratorModerationRangeWidget::range() const
{
    AdministratorModerationRangeWidget::DateRange r;
    r.fromDate = mFromDate->date().toJulianDay();
    r.toDate = mToDate->date().toJulianDay();
    return r;
}

AdministratorModerationRangeWidget::~AdministratorModerationRangeWidget() = default;

void AdministratorModerationRangeWidget::initializeMenu()
{
    auto *menu = new QMenu(mFilterDate);
    auto todayAction = new QAction(i18n("Today"), menu);
    menu->addAction(todayAction);
    connect(todayAction, &QAction::triggered, this, [this]() {
        const QDate date = QDate::currentDate();
        mFromDate->setDate(date);
        mToDate->setDate(date);
        Q_EMIT rangeChanged();
    });
    auto yesterdayAction = new QAction(i18n("Yesterday"), menu);
    menu->addAction(yesterdayAction);
    connect(yesterdayAction, &QAction::triggered, this, [this]() {
        const QDate date = QDate::currentDate().addDays(-1);
        mFromDate->setDate(date);
        mToDate->setDate(date);
        Q_EMIT rangeChanged();
    });
    auto thisWeekAction = new QAction(i18n("This week"), menu);
    menu->addAction(thisWeekAction);
    connect(thisWeekAction, &QAction::triggered, this, [this]() {
        const QDate date = QDate::currentDate();
        mFromDate->setDate(date.addDays(-7));
        mToDate->setDate(date);
        Q_EMIT rangeChanged();
    });
    auto previousWeekAction = new QAction(i18n("Previous week"), menu);
    menu->addAction(previousWeekAction);
    connect(previousWeekAction, &QAction::triggered, this, [this]() {
        const QDate date = QDate::currentDate().addDays(-7);
        mFromDate->setDate(date.addDays(-7));
        mToDate->setDate(date);
        Q_EMIT rangeChanged();
    });
    auto thisMonthAction = new QAction(i18n("This month"), menu);
    menu->addAction(thisMonthAction);
    connect(thisMonthAction, &QAction::triggered, this, [this]() {
        const QDate date = QDate::currentDate();
        const QDate monthDate = QDate(date.year(), date.month(), 1);
        mFromDate->setDate(monthDate);
        mToDate->setDate(QDate(date.year(), date.month(), monthDate.daysInMonth()));
        Q_EMIT rangeChanged();
    });
    auto previousMonthAction = new QAction(i18n("Previous month"), menu);
    menu->addAction(previousMonthAction);
    connect(previousMonthAction, &QAction::triggered, this, [this]() {
        const QDate date = QDate::currentDate().addMonths(-1);
        const QDate monthDate = QDate(date.year(), date.month(), 1);
        mFromDate->setDate(monthDate);
        mToDate->setDate(QDate(QDate(date.year(), date.month(), monthDate.daysInMonth())));
        Q_EMIT rangeChanged();
    });
    mFilterDate->setMenu(menu);
}

#include "moc_administratormoderationrangewidget.cpp"
