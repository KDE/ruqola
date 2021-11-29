/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QCheckBox;
class KFontChooser;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConfigureFontWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigureFontWidget(QWidget *parent = nullptr);
    ~ConfigureFontWidget() override;
    void save();
    void load();

private:
    QCheckBox *const mCustomFontCheck;
    KFontChooser *const mFontChooser;
};

