/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class ShowDebugWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowDebugDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ShowDebugDialog(QWidget *parent = nullptr);
    ~ShowDebugDialog() override;

    void setPlainText(const QString &text);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    ShowDebugWidget *const mShowDebugWidget;
};
