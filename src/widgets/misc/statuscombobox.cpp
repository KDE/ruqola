/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "statuscombobox.h"
#include "model/statusmodel.h"
#include "model/statusmodelfilterproxymodel.h"

StatusCombobox::StatusCombobox(QWidget *parent)
    : QComboBox(parent)
{
    setSizeAdjustPolicy(QComboBox::AdjustToContents);
}

StatusCombobox::~StatusCombobox() = default;

User::PresenceStatus StatusCombobox::status() const
{
    return currentData(StatusModel::Status).value<User::PresenceStatus>();
}

void StatusCombobox::setStatus(User::PresenceStatus status, const QString &customText)
{
    if (customText.isEmpty()) {
        setCurrentIndex(findData(QVariant::fromValue(status), StatusModel::Status));
    } else {
        // TODO use customText
        setCurrentIndex(findData(QVariant::fromValue(status), StatusModel::Status));
    }
}

void StatusCombobox::setUseOnlyStandardStatus()
{
    auto statusProxyModel = new StatusModelFilterProxyModel(this);
    statusProxyModel->setUseOnlyStandardStatus(true);
    statusProxyModel->setSourceModel(new StatusModel(this));
    setModel(statusProxyModel);
}

#include "moc_statuscombobox.cpp"
