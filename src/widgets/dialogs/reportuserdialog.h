/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_export.h"

#include <QDialog>
class ReportMessageWidget;
class LIBRUQOLAWIDGETS_EXPORT ReportUserDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ReportUserDialog(QWidget *parent = nullptr);
    ~ReportUserDialog() override;
    [[nodiscard]] QString message() const;
    void setPreviewMessage(const QString &msg);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    ReportMessageWidget *const mReportMessageWidget;
};
