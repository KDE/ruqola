/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "common/completionlineedit.h"
#include "libruqolawidgets_private_export.h"
#include "multistaticselectlineeditmodel.h"
class MultiStaticSelectLineEditProxyModel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MultiStaticSelectLineEdit : public CompletionLineEdit
{
    Q_OBJECT
public:
    explicit MultiStaticSelectLineEdit(QWidget *parent = nullptr);
    ~MultiStaticSelectLineEdit() override;

    void setSelectItemCompletionInfos(const QList<MultiStaticSelectLineEditModel::SelectItemCompletionInfo> &newUserCompletionInfos);

    void setInitialValues(const QStringList &lst);
Q_SIGNALS:
    void addSelectedItem(const MultiStaticSelectLineEditModel::SelectItemCompletionInfo &info);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void setCurrentSelectItems(const QList<MultiStaticSelectLineEditModel::SelectItemCompletionInfo> &newUserCompletionInfos);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchTextEdited();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotComplete(const QModelIndex &index);
    MultiStaticSelectLineEditModel *const mMultiStaticSelectLineEditModel;
    MultiStaticSelectLineEditProxyModel *const mMultiStaticSelectLineEditProxyModel;
};
