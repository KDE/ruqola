/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "config-ruqola.h"
#include <QLineEdit>

#include "libruqolawidgets_private_export.h"

class QAbstractItemModel;
#if HAVE_TEXTADDONSWIDGETS_COMPLETIONLINEEDIT
namespace TextAddonsWidgets
{
class CompletionListView;
}
#else
class CompletionListView;
#endif

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
#if HAVE_TEXTADDONSWIDGETS_COMPLETIONLINEEDIT
    TextAddonsWidgets::
#endif
        CompletionListView *const mCompletionListView;
};
