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

    mTimeEdit->setObjectName(u"mTimeEdit"_s);
    mainLayout->addRow(i18n("Time"), mTimeEdit);

    mTimeStampInMessagesFormatComboBox->setObjectName(u"mTimeStampInMessagesFormatComboBox"_s);
    mainLayout->addRow(i18n("Format"), mTimeStampInMessagesFormatComboBox);

    mTimeStampInMessagesTimeZoneComboBox->setObjectName(u"mTimeStampInMessagesTimeZoneComboBox"_s);
    mainLayout->addRow(i18n("TimeZone"), mTimeStampInMessagesTimeZoneComboBox);
}

TimeStampInMessagesWidget::~TimeStampInMessagesWidget() = default;

TimeStampInMessagesWidget::TimeStampInfo TimeStampInMessagesWidget::timeStampInfo() const
{
    const TimeStampInfo info{.format = mTimeStampInMessagesFormatComboBox->currentFormat(),
                             .date = mDateEdit->time().toString(),
                             .time = mTimeEdit->time().toString(),
                             .timeZone = mTimeStampInMessagesTimeZoneComboBox->currentTimeZone()};
    return info;
}

QDebug operator<<(QDebug d, const TimeStampInMessagesWidget::TimeStampInfo &t)
{
    d.space() << "format:" << t.format;
    d.space() << "time:" << t.time;
    d.space() << "date:" << t.date;
    d.space() << "timeZone:" << t.timeZone;
    return d;
}

#include "moc_timestampinmessageswidget.cpp"
