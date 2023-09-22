/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class ModerationConsoleTreeWidget;
class QDateEdit;
class QToolButton;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorModerationRangeWidget : public QWidget
{
    Q_OBJECT
public:
    struct DateRange {
        int fromDate = -1;
        int toDate = -1;
    };
    explicit AdministratorModerationRangeWidget(QWidget *parent = nullptr);
    ~AdministratorModerationRangeWidget() override;

    [[nodiscard]] DateRange range() const;

Q_SIGNALS:
    void rangeChanged();

private:
    void initializeMenu();
    QDateEdit *const mFromDate;
    QDateEdit *const mToDate;
    QToolButton *const mFilterDate;
};
