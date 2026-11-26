/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QComboBox>

class LIBRUQOLAWIDGETS_TESTS_EXPORT ClearAfterCombobox : public QComboBox
{
    Q_OBJECT
public:
    enum class Expire : int8_t {
        DontClear = 0,
        HalfHour,
        OneHour,
        Custom,
    };
    Q_ENUM(Expire)
    explicit ClearAfterCombobox(QWidget *parent = nullptr);
    ~ClearAfterCombobox() override;

Q_SIGNALS:
    void customRequested();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void init();
};
