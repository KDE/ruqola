/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "timestampinmessageswidget.h"
#include <KLocalizedString>
#include <QDateEdit>
#include <QFormLayout>
using namespace Qt::Literals::StringLiterals;
TimeStampInMessagesWidget::TimeStampInMessagesWidget(QWidget *parent)
    : QWidget{parent}
    , mDateEdit(new QDateEdit(this))
    , mTimeEdit(new QTimeEdit(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mDateEdit->setObjectName(u"mDateEdit"_s);
    mainLayout->addRow(i18n("Date"), mDateEdit);

    mTimeEdit->setObjectName(u"mTimeEdit"_s);
    mainLayout->addRow(i18n("Time"), mTimeEdit);
}

TimeStampInMessagesWidget::~TimeStampInMessagesWidget() = default;

#include "moc_timestampinmessageswidget.cpp"
