/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLabel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ReadOnlyLineEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ReadOnlyLineEditWidget(QWidget *parent = nullptr);
    ~ReadOnlyLineEditWidget() override;
    void setMessage(const QString &str);

private:
    QLabel *const mLabel;
};
