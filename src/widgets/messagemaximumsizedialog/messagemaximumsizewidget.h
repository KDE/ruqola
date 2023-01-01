/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

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

    Q_REQUIRED_RESULT QString description() const;

    Q_REQUIRED_RESULT QString fileName() const;
Q_SIGNALS:
    void updateOkButton(bool enabled);

private:
    void slotTextChanged();
    QLineEdit *const mFileName;
    QLineEdit *const mDescription;
};
