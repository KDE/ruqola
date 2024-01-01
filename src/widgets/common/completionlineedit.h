/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QLineEdit>

#include "libruqolawidgets_private_export.h"

class QAbstractItemModel;
class CompletionListView;

class LIBRUQOLAWIDGETS_TESTS_EXPORT CompletionLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit CompletionLineEdit(QWidget *parent = nullptr);
    ~CompletionLineEdit() override;

    void setCompletionModel(QAbstractItemModel *model);

Q_SIGNALS:
    void complete(const QModelIndex &index);

protected:
    CompletionListView *const mCompletionListView;
};
