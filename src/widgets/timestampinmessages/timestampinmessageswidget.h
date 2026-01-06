/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QDateEdit;
class QTimeEdit;
class TimeStampInMessagesFormatComboBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT TimeStampInMessagesWidget : public QWidget
{
    Q_OBJECT
public:
    struct TimeStampInfo {
        // TODO
    };

    explicit TimeStampInMessagesWidget(QWidget *parent = nullptr);
    ~TimeStampInMessagesWidget() override;

private:
    QDateEdit *const mDateEdit;
    QTimeEdit *const mTimeEdit;
    TimeStampInMessagesFormatComboBox *const mTimeStampInMessagesFormatComboBox;
};
