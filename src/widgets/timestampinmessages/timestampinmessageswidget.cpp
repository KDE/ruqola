/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "timestampinmessageswidget.h"
#include "timestampinmessages/timestampinmessagesformatcombobox.h"
#include "timestampinmessages/timestampinmessagestimezonecombobox.h"
#include <KLocalizedString>
#include <QDateEdit>
#include <QFormLayout>
using namespace Qt::Literals::StringLiterals;
TimeStampInMessagesWidget::TimeStampInMessagesWidget(QWidget *parent)
    : QWidget{parent}
    , mDateEdit(new QDateEdit(this))
    , mTimeEdit(new QTimeEdit(this))
    , mTimeStampInMessagesFormatComboBox(new TimeStampInMessagesFormatComboBox(this))
    , mTimeStampInMessagesTimeZoneComboBox(new TimeStampInMessagesTimeZoneComboBox(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mDateEdit->setObjectName(u"mDateEdit"_s);
    mainLayout->addRow(i18n("Date"), mDateEdit);
    mDateEdit->setDate(QDate::currentDate());

    mTimeEdit->setObjectName(u"mTimeEdit"_s);
    mainLayout->addRow(i18n("Time"), mTimeEdit);
    mTimeEdit->setTime(QTime::currentTime());

    mTimeStampInMessagesFormatComboBox->setObjectName(u"mTimeStampInMessagesFormatComboBox"_s);
    mainLayout->addRow(i18n("Format"), mTimeStampInMessagesFormatComboBox);

    mTimeStampInMessagesTimeZoneComboBox->setObjectName(u"mTimeStampInMessagesTimeZoneComboBox"_s);
    mainLayout->addRow(i18n("TimeZone"), mTimeStampInMessagesTimeZoneComboBox);
}

TimeStampInMessagesWidget::~TimeStampInMessagesWidget() = default;

TimeStampInMessagesUtils::TimeStampInfo TimeStampInMessagesWidget::timeStampInfo() const
{
    const TimeStampInMessagesUtils::TimeStampInfo info{.format = mTimeStampInMessagesFormatComboBox->currentFormat(),
                                                       .date = mDateEdit->date(),
                                                       .time = mTimeEdit->time(),
                                                       .timeZone = mTimeStampInMessagesTimeZoneComboBox->currentTimeZone()};
    return info;
}

#include "moc_timestampinmessageswidget.cpp"
