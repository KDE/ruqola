/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QCheckBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConfigureGeneralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigureGeneralWidget(QWidget *parent = nullptr);
    ~ConfigureGeneralWidget() override;

    void save();
    void load();

private:
    QCheckBox *const mSetOnlineForAllAccount;
    QCheckBox *const mShowImageByDefault;
    QCheckBox *const mMarkAsReadOnTextClicked;
    QCheckBox *const mEnableSystemTray;
    QCheckBox *const mEnableLogging;
    QCheckBox *const mShowHoverHightLights;
};
