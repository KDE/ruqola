/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "emoticonlistviewbase.h"
#include "libruqolawidgets_private_export.h"

class LIBRUQOLAWIDGETS_TESTS_EXPORT EmoticonListView : public EmoticonListViewBase
{
    Q_OBJECT
public:
    explicit EmoticonListView(QWidget *parent = nullptr);
    ~EmoticonListView() override;
Q_SIGNALS:
    void emojiItemSelected(const QString &str);
};
