/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "delegateutils/textselection.h"
#include "libruqolawidgets_private_export.h"
#include <QItemDelegate>
class TextSelectionImpl;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageListDelegateBase : public QItemDelegate, public DocumentFactoryInterface
{
    Q_OBJECT
public:
    explicit MessageListDelegateBase(QObject *parent = nullptr);
    ~MessageListDelegateBase() override;

Q_SIGNALS:
    void updateView(const QModelIndex &index);

protected:
    TextSelectionImpl *const mTextSelectionImpl;
};
