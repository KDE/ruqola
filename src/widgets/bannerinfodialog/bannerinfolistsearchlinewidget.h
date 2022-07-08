/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QLineEdit;
class QCheckBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT BannerInfoListSearchLineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BannerInfoListSearchLineWidget(QWidget *parent = nullptr);
    ~BannerInfoListSearchLineWidget() override;

    Q_REQUIRED_RESULT QString searchText() const;

private:
    QLineEdit *const mSearchLineEdit;
    QCheckBox *const mUnrealCheckBox;
};
