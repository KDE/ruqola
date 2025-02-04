/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] QString searchText() const;

    [[nodiscard]] bool showOnlyUnread() const;

Q_SIGNALS:
    void filterChanged();

private:
    QLineEdit *const mSearchLineEdit;
    QCheckBox *const mOnlyUnReadCheckBox;
};
