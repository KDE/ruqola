/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_export.h"

#include <QDialog>
class ReportUserWidget;
class LIBRUQOLAWIDGETS_EXPORT ReportUserDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ReportUserDialog(QWidget *parent = nullptr);
    ~ReportUserDialog() override;
    [[nodiscard]] QString message() const;

    void setUserName(const QString &userName);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    ReportUserWidget *const mReportUserWidget;
};
