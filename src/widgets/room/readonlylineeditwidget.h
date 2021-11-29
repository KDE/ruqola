/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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

