/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textuibase.h"

TextUiBase::TextUiBase(TextSelectionImpl *textSelectionImpl, QListView *view)
    : mTextSelectionImpl(textSelectionImpl)
    , mListView(view)
{
}

TextUiBase::~TextUiBase() = default;
