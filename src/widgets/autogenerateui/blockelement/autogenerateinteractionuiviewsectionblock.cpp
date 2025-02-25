/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewsectionblock.h"
#include "autogenerateui/autogenerateinteractionuiviewtext.h"
#include <QLabel>
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewSectionBlock::AutoGenerateInteractionUiViewSectionBlock()
    : AutoGenerateInteractionUiViewBlockBase()
{
}

AutoGenerateInteractionUiViewSectionBlock::~AutoGenerateInteractionUiViewSectionBlock()
{
    delete mText;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewSectionBlock &t)
{
    if (t.text()) {
        d.space() << "text:" << *t.text();
    }
    return d;
}

bool AutoGenerateInteractionUiViewSectionBlock::operator==(const AutoGenerateInteractionUiViewSectionBlock &other) const
{
    return AutoGenerateInteractionUiViewBlockBase::operator==(other);
}

QWidget *AutoGenerateInteractionUiViewSectionBlock::generateWidget(QWidget *parent) const
{
    if (mText) {
        auto label = new QLabel(parent);
        label->setText(mText->generateText());
        return label;
    }
    return nullptr;
}

AutoGenerateInteractionUiViewText *AutoGenerateInteractionUiViewSectionBlock::text() const
{
    return mText;
}

void AutoGenerateInteractionUiViewSectionBlock::setText(AutoGenerateInteractionUiViewText *newText)
{
    mText = newText;
}

void AutoGenerateInteractionUiViewSectionBlock::parse(const QJsonObject &json)
{
    if (json.contains("text"_L1)) {
        mText = new AutoGenerateInteractionUiViewText;
        mText->parse(json["text"_L1].toObject());
    }
}
