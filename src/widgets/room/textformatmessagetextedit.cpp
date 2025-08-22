/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textformatmessagetextedit.h"

#include <QHBoxLayout>

TextFormatMessageTextEdit::TextFormatMessageTextEdit(QWidget *parent)
    : QFrame(parent)
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);
}

TextFormatMessageTextEdit::~TextFormatMessageTextEdit() = default;
