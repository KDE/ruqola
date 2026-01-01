/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "multistaticselectwidget.h"
#include "common/flowlayout.h"
#include "misc/clickablewidget.h"
#include "multistaticselectlineedit.h"
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
MultiStaticSelectWidget::MultiStaticSelectWidget(QWidget *parent)
    : QWidget{parent}
    , mLineEdit(new MultiStaticSelectLineEdit(this))
    , mFlowLayout(new FlowLayout)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});

    mLineEdit->setObjectName("mLineEdit"_L1);
    mainLayout->addWidget(mLineEdit);

    mFlowLayout->setObjectName(u"mFlowLayout"_s);
    mainLayout->addLayout(mFlowLayout);
    connect(mLineEdit, &MultiStaticSelectLineEdit::addSelectedItem, this, &MultiStaticSelectWidget::slotAddNewName);
}

MultiStaticSelectWidget::~MultiStaticSelectWidget() = default;

void MultiStaticSelectWidget::setUserCompletionInfos(const QList<MultiStaticSelectLineEditModel::SelectItemCompletionInfo> &newUserCompletionInfos)
{
    mLineEdit->setSelectItemCompletionInfos(newUserCompletionInfos);
}

QStringList MultiStaticSelectWidget::selectedUsers() const
{
    QStringList addUsers;
    for (const auto &[key, value] : mMap.asKeyValueRange()) {
        addUsers << QString::fromUtf8(value->identifier());
    }
    return addUsers;
}

void MultiStaticSelectWidget::setMaxSelectItems(int maxSelectItems)
{
    mMaxSelectItems = maxSelectItems;
}

void MultiStaticSelectWidget::setInitialValues(const QStringList &lst)
{
    mLineEdit->setInitialValues(lst);
}

void MultiStaticSelectWidget::setPlaceholderText(const QString &str)
{
    mLineEdit->setPlaceholderText(str);
}

void MultiStaticSelectWidget::slotAddNewName(const MultiStaticSelectLineEditModel::SelectItemCompletionInfo &info)
{
    const QString &userName = info.text;
    if (mMap.contains(userName)) {
        return;
    }
    auto clickableUserWidget = new ClickableWidget(userName, this);
    clickableUserWidget->setIdentifier(info.value.toLatin1());
    connect(clickableUserWidget, &ClickableWidget::removeClickableWidget, this, &MultiStaticSelectWidget::slotRemoveUser);
    mFlowLayout->addWidget(clickableUserWidget);
    mMap.insert(userName, clickableUserWidget);
    checkMaxSelectedItems();
}

void MultiStaticSelectWidget::checkMaxSelectedItems()
{
    if (mMaxSelectItems != -1) {
        mLineEdit->setEnabled(mMap.count() < mMaxSelectItems);
    }
}

void MultiStaticSelectWidget::slotRemoveUser(const QString &username)
{
    ClickableWidget *userWidget = mMap.value(username);
    if (userWidget) {
        const int index = mFlowLayout->indexOf(userWidget);
        if (index != -1) {
            userWidget->deleteLater();
            delete mFlowLayout->takeAt(index);
            mMap.remove(username);
            checkMaxSelectedItems();
        }
    }
}

#include "moc_multistaticselectwidget.cpp"
