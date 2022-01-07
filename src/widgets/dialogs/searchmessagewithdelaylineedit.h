/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include "misc/searchwithdelaylineedit.h"
class QCompleter;
class QStringListModel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT SearchMessageWithDelayLineEdit : public SearchWithDelayLineEdit
{
    Q_OBJECT
public:
    explicit SearchMessageWithDelayLineEdit(QWidget *parent = nullptr);
    ~SearchMessageWithDelayLineEdit() override;
    void addCompletionItem(const QString &str);

private:
    QStringList mListCompetion;
    QCompleter *const mCompleter;
    QStringListModel *const mCompleterListModel;
};
