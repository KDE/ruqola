/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textselectionimpl.h"
#include "textselection.h"

TextSelectionImpl::TextSelectionImpl()
    : mTextSelection(new TextSelection)
{
}

TextSelectionImpl::~TextSelectionImpl()
{
    delete mTextSelection;
}

TextSelection *TextSelectionImpl::textSelection() const
{
    return mTextSelection;
}

bool TextSelectionImpl::mightStartDrag() const
{
    return mMightStartDrag;
}

void TextSelectionImpl::setMightStartDrag(bool newMightStartDrag)
{
    mMightStartDrag = newMightStartDrag;
}
