/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QListView>

class LIBRUQOLAWIDGETS_TESTS_EXPORT EmoticonListView : public QListView
{
    Q_OBJECT
public:
    explicit EmoticonListView(QWidget *parent = nullptr);
    ~EmoticonListView() override;
Q_SIGNALS:
    void emojiItemSelected(const QString &str);
};
