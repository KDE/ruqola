/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <KTextEdit>

#include "libruqolawidgets_private_export.h"

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageTextEditor : public KTextEdit
{
    Q_OBJECT
public:
    explicit MessageTextEditor(QWidget *parent = nullptr);
    ~MessageTextEditor() override;

    Q_REQUIRED_RESULT QSize sizeHint() const override;
    Q_REQUIRED_RESULT QSize minimumSizeHint() const override;
};
