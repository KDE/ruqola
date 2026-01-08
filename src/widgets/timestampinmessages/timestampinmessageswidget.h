/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QDebug;
class QDateEdit;
class QTimeEdit;
class TimeStampInMessagesFormatComboBox;
class TimeStampInMessagesTimeZoneComboBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT TimeStampInMessagesWidget : public QWidget
{
    Q_OBJECT
public:
    struct TimeStampInfo {
        QString format;
        QString date;
        QString time;
        QString timeZone;
    };

    explicit TimeStampInMessagesWidget(QWidget *parent = nullptr);
    ~TimeStampInMessagesWidget() override;

    [[nodiscard]] TimeStampInfo timeStampInfo() const;

private:
    QDateEdit *const mDateEdit;
    QTimeEdit *const mTimeEdit;
    TimeStampInMessagesFormatComboBox *const mTimeStampInMessagesFormatComboBox;
    TimeStampInMessagesTimeZoneComboBox *const mTimeStampInMessagesTimeZoneComboBox;
};
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const TimeStampInMessagesWidget::TimeStampInfo &t);
