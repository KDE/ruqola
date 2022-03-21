/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageMaximumSizeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MessageMaximumSizeWidget(QWidget *parent = nullptr);
    ~MessageMaximumSizeWidget() override;
Q_SIGNALS:
    void updateOkButton(bool enabled);

private:
    QLineEdit *const mFileName;
    QLineEdit *const mDescription;
};
