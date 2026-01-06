/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QComboBox>

class LIBRUQOLAWIDGETS_TESTS_EXPORT TimeStampInMessagesFormatComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit TimeStampInMessagesFormatComboBox(QWidget *parent = nullptr);
    ~TimeStampInMessagesFormatComboBox() override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void initialize();
};
