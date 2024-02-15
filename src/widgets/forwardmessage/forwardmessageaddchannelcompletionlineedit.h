/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "common/completionlineedit.h"
#include "libruqolawidgets_private_export.h"

class LIBRUQOLAWIDGETS_TESTS_EXPORT ForwardMessageAddChannelCompletionLineEdit : public CompletionLineEdit
{
    Q_OBJECT
public:
    explicit ForwardMessageAddChannelCompletionLineEdit(QWidget *parent = nullptr);
    ~ForwardMessageAddChannelCompletionLineEdit() override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotComplete(const QModelIndex &index);
};
