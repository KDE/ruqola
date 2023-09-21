/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QDialog>
#include <QObject>
class RemoveAccountWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RemoveAccountDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RemoveAccountDialog(QWidget *parent = nullptr);
    ~RemoveAccountDialog() override;

    [[nodiscard]] bool removeLogs() const;

    void setAccountName(const QString &accountName);

private:
    RemoveAccountWidget *const mRemoveAccountWidget;
};
