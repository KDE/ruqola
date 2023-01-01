/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QComboBox>
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowAttachmentComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ShowAttachmentComboBox(QWidget *parent = nullptr);
    ~ShowAttachmentComboBox() override;

private:
    void fillCombobox();
};
