/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QComboBox>

class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsCategoriesComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsCategoriesComboBox(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ApplicationsSettingsCategoriesComboBox() override;

    [[nodiscard]] QStringList categories() const;

Q_SIGNALS:
    void categoriesChanged();

private:
    RocketChatAccount *const mRocketChatAccount;
};
