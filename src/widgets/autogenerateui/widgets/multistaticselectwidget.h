/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "multistaticselectlineeditmodel.h"
#include <QWidget>
class MultiStaticSelectLineEdit;
class FlowLayout;
class ClickableWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MultiStaticSelectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MultiStaticSelectWidget(QWidget *parent = nullptr);
    ~MultiStaticSelectWidget() override;
    [[nodiscard]] QStringList selectedUsers() const;

    void setPlaceholderText(const QString &str);

    void setUserCompletionInfos(const QList<MultiStaticSelectLineEditModel::SelectItemCompletionInfo> &newUserCompletionInfos);

    void setMaxSelectItems(int maxSelectItems);
    void setInitialValues(const QStringList &lst);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAddNewName(const MultiStaticSelectLineEditModel::SelectItemCompletionInfo &info);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRemoveUser(const QString &username);
    LIBRUQOLAWIDGETS_NO_EXPORT void checkMaxSelectedItems();
    MultiStaticSelectLineEdit *const mLineEdit;
    FlowLayout *const mFlowLayout;
    QMap<QString, ClickableWidget *> mMap;
    int mMaxSelectItems = -1;
};
