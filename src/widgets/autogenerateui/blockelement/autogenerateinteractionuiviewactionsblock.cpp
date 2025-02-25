/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewactionsblock.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewbuttonelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewcheckboxelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewradiobuttonelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewtoggleswitchelement.h"
#include "ruqola_autogenerateui_debug.h"
#include <QJsonArray>
#include <QLayout>
#include <QWidget>
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewActionsBlock::AutoGenerateInteractionUiViewActionsBlock()
    : AutoGenerateInteractionUiViewBlockBase()
{
}

AutoGenerateInteractionUiViewActionsBlock::~AutoGenerateInteractionUiViewActionsBlock()
{
    qDeleteAll(mElements);
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewActionsBlock &t)
{
    d << "elements:" << t.elements();
    return d;
}

bool AutoGenerateInteractionUiViewActionsBlock::operator==(const AutoGenerateInteractionUiViewActionsBlock &other) const
{
    return AutoGenerateInteractionUiViewBlockBase::operator==(other);
}

void AutoGenerateInteractionUiViewActionsBlock::parse(const QJsonObject &json)
{
    /*
                | ChannelsSelectElement
                | ConversationsSelectElement
                | DatePickerElement
                | LinearScaleElement
                | MultiChannelsSelectElement
                | MultiConversationsSelectElement
                | MultiStaticSelectElement
                | MultiUsersSelectElement
                | OverflowElement
                | StaticSelectElement
                | UsersSelectElement
                | TimePickerElement
*/
    AutoGenerateInteractionUiViewBlockBase::parse(json);
    for (const auto &r : json["elements"_L1].toArray()) {
        const QString type = r["type"_L1].toString();
        if (type == "button"_L1) {
            AutoGenerateInteractionUiViewButtonElement *e = new AutoGenerateInteractionUiViewButtonElement;
            e->parse(r.toObject());
            mElements.append(std::move(e));
        } else if (type == "checkbox"_L1) {
            AutoGenerateInteractionUiViewCheckboxElement *e = new AutoGenerateInteractionUiViewCheckboxElement;
            e->parse(r.toObject());
            mElements.append(std::move(e));
        } else if (type == "radio_button"_L1) {
            AutoGenerateInteractionUiViewRadioButtonElement *e = new AutoGenerateInteractionUiViewRadioButtonElement;
            e->parse(r.toObject());
            mElements.append(std::move(e));
        } else if (type == "toggle_switch"_L1) {
            AutoGenerateInteractionUiViewToggleSwitchElement *e = new AutoGenerateInteractionUiViewToggleSwitchElement;
            e->parse(r.toObject());
            mElements.append(std::move(e));
        } else {
            qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "Unknown type " << type;
        }
    }
}

QList<AutoGenerateInteractionUiViewActionable *> AutoGenerateInteractionUiViewActionsBlock::elements() const
{
    return mElements;
}

void AutoGenerateInteractionUiViewActionsBlock::setElements(const QList<AutoGenerateInteractionUiViewActionable *> &newElements)
{
    mElements = newElements;
}

QWidget *AutoGenerateInteractionUiViewActionsBlock::generateWidget(QWidget *parent) const
{
    auto widget = new QWidget(parent);
    parent->layout()->addWidget(widget);
    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setContentsMargins({});
    widget->setLayout(hboxLayout);
    for (const auto &e : std::as_const(mElements)) {
        hboxLayout->addWidget(e->generateWidget(parent));
    }
    hboxLayout->addStretch(1);
    return widget;
}
