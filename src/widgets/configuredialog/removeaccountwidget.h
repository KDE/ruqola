/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QCheckBox;
class QLabel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RemoveAccountWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RemoveAccountWidget(QWidget *parent = nullptr);
    ~RemoveAccountWidget() override;
    [[nodiscard]] bool removeLogs() const;

    void setAccountName(const QString &accountName);

private:
    QCheckBox *const mRemoveLogs;
    QLabel *const mInfo;
};
