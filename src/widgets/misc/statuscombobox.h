/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "user.h"
#include <QComboBox>

class LIBRUQOLAWIDGETS_TESTS_EXPORT StatusCombobox : public QComboBox
{
    Q_OBJECT
public:
    explicit StatusCombobox(QWidget *parent = nullptr);
    ~StatusCombobox() override;
    [[nodiscard]] User::PresenceStatus status() const;
    void setStatus(User::PresenceStatus status, const QString &customText = {});
    void setUseOnlyStandardStatus();
};
