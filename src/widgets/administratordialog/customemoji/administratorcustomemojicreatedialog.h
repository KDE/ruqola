/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "administratorcustomemojicreatewidget.h"
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorCustomEmojiCreateDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AdministratorCustomEmojiCreateDialog(QWidget *parent = nullptr);
    ~AdministratorCustomEmojiCreateDialog() override;

    void setCustomEmojiInfo(const AdministratorCustomEmojiCreateWidget::CustomEmojiCreateInfo &info);
    Q_REQUIRED_RESULT AdministratorCustomEmojiCreateWidget::CustomEmojiCreateInfo info() const;

private:
    void readConfig();
    void writeConfig();
    AdministratorCustomEmojiCreateWidget *const mCreateWidget;
};
