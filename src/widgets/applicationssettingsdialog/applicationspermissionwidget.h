/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QTextEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsPermissionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ApplicationsPermissionWidget(QWidget *parent = nullptr);
    ~ApplicationsPermissionWidget() override;
    void setApplicationPermission(const QString &desc);

private:
    QTextEdit *const mTextEdit;
};
