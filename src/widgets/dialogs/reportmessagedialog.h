/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_export.h"

#include <QDialog>
class ReportMessageWidget;
class LIBRUQOLAWIDGETS_EXPORT ReportMessageDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ReportMessageDialog(QWidget *parent = nullptr);
    ~ReportMessageDialog() override;
    Q_REQUIRED_RESULT QString message() const;
    void setPreviewMessage(const QString &msg);

private:
    void readConfig();
    void writeConfig();
    ReportMessageWidget *const mReportMessageWidget;
};
