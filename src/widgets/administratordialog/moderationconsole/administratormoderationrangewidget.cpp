/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratormoderationrangewidget.h"
using namespace Qt::Literals::StringLiterals;

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
    mainLayout->setObjectName(u"mainLayout"_s);

    mFromDate->setObjectName(u"mFromDate"_s);
    mToDate->setObjectName(u"mToDate"_s);

    mFilterDate->setObjectName(u"mFilterDate"_s);
    mFilterDate->setAutoRaise(true);
    mFilterDate->setPopupMode(QToolButton::InstantPopup);
    mFilterDate->setIcon(QIcon::fromTheme(u"application-menu"_s));

    auto fromLabel = new QLabel(i18nc("@label:textbox", "From:"), this);
    fromLabel->setObjectName(u"fromLabel"_s);

    auto toLabel = new QLabel(i18nc("@label:textbox", "To:"), this);
    toLabel->setObjectName(u"toLabel"_s);

    mainLayout->addWidget(fromLabel);
    mainLayout->addWidget(mFromDate);
    mainLayout->addWidget(toLabel);
    mainLayout->addWidget(mToDate);
    mainLayout->addWidget(mFilterDate);
    mainLayout->addStretch();
    initializeMenu();
    connect(mFromDate, &QDateEdit::dateChanged, this, &AdministratorModerationRangeWidget::slotRangeChanged);
    connect(mToDate, &QDateEdit::dateChanged, this, &AdministratorModerationRangeWidget::slotRangeChanged);
}

AdministratorModerationRangeWidget::~AdministratorModerationRangeWidget() = default;

AdministratorModerationRangeWidget::DateTimeRange AdministratorModerationRangeWidget::range() const
{
    AdministratorModerationRangeWidget::DateTimeRange r;
    r.fromDate = QDateTime(mFromDate->date(), QTime(0, 0, 0));
    r.toDate = QDateTime(mToDate->date(), QTime(23, 59, 59));
    return r;
}

void AdministratorModerationRangeWidget::slotRangeChanged()
{
    const QDate fromDate = mFromDate->date();
    const QDate toDate = mToDate->date();
    if (fromDate > toDate) {
        QSignalBlocker b(mFromDate);
        QSignalBlocker b2(mToDate);
        mToDate->setDate(fromDate);
        mFromDate->setDate(toDate);
    }
    Q_EMIT rangeChanged();
}

void AdministratorModerationRangeWidget::initializeMenu()
{
    auto menu = new QMenu(mFilterDate);
    auto todayAction = new QAction(i18nc("@action", "Today"), menu);
    menu->addAction(todayAction);
    connect(todayAction, &QAction::triggered, this, [this]() {
        const QDate date = QDate::currentDate();
        QSignalBlocker b(mFromDate);
        QSignalBlocker b2(mToDate);
        mFromDate->setDate(date);
        mToDate->setDate(date);
        Q_EMIT rangeChanged();
    });
    auto yesterdayAction = new QAction(i18nc("@action", "Yesterday"), menu);
    menu->addAction(yesterdayAction);
    connect(yesterdayAction, &QAction::triggered, this, [this]() {
        QSignalBlocker b(mFromDate);
        QSignalBlocker b2(mToDate);
        const QDate date = QDate::currentDate().addDays(-1);
        mFromDate->setDate(date);
        mToDate->setDate(date);
        Q_EMIT rangeChanged();
    });
    auto thisWeekAction = new QAction(i18nc("@action", "This week"), menu);
    menu->addAction(thisWeekAction);
    connect(thisWeekAction, &QAction::triggered, this, [this]() {
        QSignalBlocker b(mFromDate);
        QSignalBlocker b2(mToDate);
        const QDate date = QDate::currentDate();
        mFromDate->setDate(date.addDays(-7));
        mToDate->setDate(date);
        Q_EMIT rangeChanged();
    });
    auto previousWeekAction = new QAction(i18nc("@action", "Previous week"), menu);
    menu->addAction(previousWeekAction);
    connect(previousWeekAction, &QAction::triggered, this, [this]() {
        QSignalBlocker b(mFromDate);
        QSignalBlocker b2(mToDate);
        const QDate date = QDate::currentDate().addDays(-7);
        mFromDate->setDate(date.addDays(-7));
        mToDate->setDate(date);
        Q_EMIT rangeChanged();
    });
    auto thisMonthAction = new QAction(i18nc("@action", "This month"), menu);
    menu->addAction(thisMonthAction);
    connect(thisMonthAction, &QAction::triggered, this, [this]() {
        QSignalBlocker b(mFromDate);
        QSignalBlocker b2(mToDate);
        const QDate date = QDate::currentDate();
        const QDate monthDate = QDate(date.year(), date.month(), 1);
        mFromDate->setDate(monthDate);
        mToDate->setDate(QDate(date.year(), date.month(), monthDate.daysInMonth()));
        Q_EMIT rangeChanged();
    });
    auto previousMonthAction = new QAction(i18nc("@action", "Previous month"), menu);
    menu->addAction(previousMonthAction);
    connect(previousMonthAction, &QAction::triggered, this, [this]() {
        QSignalBlocker b(mFromDate);
        QSignalBlocker b2(mToDate);
        const QDate date = QDate::currentDate().addMonths(-1);
        const QDate monthDate = QDate(date.year(), date.month(), 1);
        mFromDate->setDate(monthDate);
        mToDate->setDate(QDate(QDate(date.year(), date.month(), monthDate.daysInMonth())));
        Q_EMIT rangeChanged();
    });
    mFilterDate->setMenu(menu);
}

QDebug operator<<(QDebug d, const AdministratorModerationRangeWidget::DateTimeRange &t)
{
    d.space() << "fromDate : " << t.fromDate;
    d.space() << "toDate : " << t.toDate;
    return d;
}

bool AdministratorModerationRangeWidget::DateTimeRange::isValid() const
{
    return fromDate.isValid() && toDate.isValid();
}

#include "moc_administratormoderationrangewidget.cpp"
