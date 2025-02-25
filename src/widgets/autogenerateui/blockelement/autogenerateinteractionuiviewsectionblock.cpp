/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewsectionblock.h"
#include "autogenerateui/autogenerateinteractionuiviewtext.h"
#include "ruqola_autogenerateui_debug.h"
#include <QLabel>
#include <QLayout>
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
    /*
    if (t.accessory()) {
        d.space() << "accessory:" << *t.accessory();
    }
    */
    return d;
}

bool AutoGenerateInteractionUiViewSectionBlock::operator==(const AutoGenerateInteractionUiViewSectionBlock &other) const
{
    return AutoGenerateInteractionUiViewBlockBase::operator==(other) && other.text() == text() && other.accessory() == other.accessory();
}

QWidget *AutoGenerateInteractionUiViewSectionBlock::generateWidget(QWidget *parent) const
{
    auto widget = new QWidget(parent);
    parent->layout()->addWidget(widget);
    auto vboxLayout = new QVBoxLayout;
    vboxLayout->setContentsMargins({});
    widget->setLayout(vboxLayout);

    if (mText) {
        auto label = new QLabel(parent);
        label->setText(mText->generateText());
        vboxLayout->addWidget(label);
    }
    if (mAccessory) {
        // TODO
    }
    return widget;
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
    if (json.contains("accessory"_L1)) {
        const QJsonObject accessoryObj = json["accessory"_L1].toObject();
        const QString type = accessoryObj["type"_L1].toString();
        if (type == "button"_L1) {
        } else if (type == "datepicker"_L1) {
            qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "AutoGenerateInteractionUiViewInputBlock not implemented yet " << type;
        } else if (type == "image"_L1) {
            qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "AutoGenerateInteractionUiViewInputBlock not implemented yet " << type;
        } else if (type == "multi_static_select"_L1) {
            qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "AutoGenerateInteractionUiViewInputBlock not implemented yet " << type;
        } else if (type == "overflow"_L1) {
            qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "AutoGenerateInteractionUiViewInputBlock not implemented yet " << type;
        } else if (type == "static_select"_L1) {
            qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "AutoGenerateInteractionUiViewInputBlock not implemented yet " << type;
        } else {
            qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "AutoGenerateInteractionUiViewInputBlock Unknown type " << type;
        }
    }
}

AutoGenerateInteractionUiViewActionable *AutoGenerateInteractionUiViewSectionBlock::accessory() const
{
    return mAccessory;
}

void AutoGenerateInteractionUiViewSectionBlock::setAccessory(AutoGenerateInteractionUiViewActionable *newAccessory)
{
    mAccessory = newAccessory;
}
