/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorcustomemojicreateorupdatewidget.h"
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorCustomEmojiCreateOrUpdateDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AdministratorCustomEmojiCreateOrUpdateDialog(QWidget *parent = nullptr);
    ~AdministratorCustomEmojiCreateOrUpdateDialog() override;

    void setCustomEmojiInfo(const AdministratorCustomEmojiCreateOrUpdateWidget::CustomEmojiCreateInfo &info);
    Q_REQUIRED_RESULT AdministratorCustomEmojiCreateOrUpdateWidget::CustomEmojiCreateInfo info() const;

    Q_REQUIRED_RESULT AdministratorCustomEmojiCreateOrUpdateWidget::AdministratorCustomEmojiCreateOrUpdateType type() const;
    void setType(AdministratorCustomEmojiCreateOrUpdateWidget::AdministratorCustomEmojiCreateOrUpdateType newType);

private:
    void readConfig();
    void writeConfig();
    AdministratorCustomEmojiCreateOrUpdateWidget *const mCreateWidget;
};
