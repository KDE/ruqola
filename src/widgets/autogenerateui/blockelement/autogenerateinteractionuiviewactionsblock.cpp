/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewactionsblock.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewbuttonelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewcheckboxelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewdatepickerelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewlinearscaleelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewoverflowelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewradiobuttonelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewstaticselectelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewtimepickerelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewtoggleswitchelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewuserselectelement.h"
#include "autogenerateui/widgets/actionelementwidget.h"

#include "common/flowlayout.h"
#include "ruqola_autogenerateui_debug.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QLayout>
#include <QWidget>
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewActionsBlock::AutoGenerateInteractionUiViewActionsBlock(QObject *parent)
    : AutoGenerateInteractionUiViewBlockBase(parent)
{
}

AutoGenerateInteractionUiViewActionsBlock::~AutoGenerateInteractionUiViewActionsBlock()
{
    qDeleteAll(mElements);
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewActionsBlock &t)
{
    d.space() << "AutoGenerateInteractionUiViewBlockBase:" << static_cast<const AutoGenerateInteractionUiViewBlockBase &>(t);
    d.space() << "elements:" << t.elements();
    return d;
}

bool AutoGenerateInteractionUiViewActionsBlock::operator==(const AutoGenerateInteractionUiViewActionsBlock &other) const
{
    return AutoGenerateInteractionUiViewBlockBase::operator==(other);
}

void AutoGenerateInteractionUiViewActionsBlock::parseBlock(const QJsonObject &json)
{
    /*
                | ChannelsSelectElement
                | ConversationsSelectElement
                | MultiChannelsSelectElement
                | MultiConversationsSelectElement
                | MultiUsersSelectElement
*/
    const QJsonArray array = json["elements"_L1].toArray();
    for (const auto &r : array) {
        const QString type = r["type"_L1].toString();
        if (type == "button"_L1) {
            auto e = new AutoGenerateInteractionUiViewButtonElement;
            e->parse(r.toObject());
            mElements.append(std::move(e));
        } else if (type == "checkbox"_L1) {
            auto e = new AutoGenerateInteractionUiViewCheckboxElement;
            e->parse(r.toObject());
            mElements.append(std::move(e));
        } else if (type == "radio_button"_L1) {
            auto e = new AutoGenerateInteractionUiViewRadioButtonElement;
            e->parse(r.toObject());
            mElements.append(std::move(e));
        } else if (type == "toggle_switch"_L1) {
            auto e = new AutoGenerateInteractionUiViewToggleSwitchElement;
            e->parse(r.toObject());
            mElements.append(std::move(e));
        } else if (type == "static_select"_L1) {
            auto e = new AutoGenerateInteractionUiViewStaticSelectElement;
            e->parse(r.toObject());
            mElements.append(std::move(e));
        } else if (type == "overflow"_L1) {
            auto e = new AutoGenerateInteractionUiViewOverflowElement;
            e->parse(r.toObject());
            mElements.append(std::move(e));
        } else if (type == "linear_scale"_L1) {
            auto e = new AutoGenerateInteractionUiViewLinearScaleElement;
            e->parse(r.toObject());
            mElements.append(std::move(e));
        } else if (type == "datepicker"_L1) {
            auto e = new AutoGenerateInteractionUiViewDatePickerElement;
            e->parse(r.toObject());
            mElements.append(std::move(e));
        } else if (type == "time_picker"_L1) {
            auto e = new AutoGenerateInteractionUiViewTimePickerElement;
            e->parse(r.toObject());
            mElements.append(std::move(e));
        } else if (type == "users_select"_L1) {
            auto e = new AutoGenerateInteractionUiViewUserSelectElement;
            e->parse(r.toObject());
            mElements.append(std::move(e));
        } else {
            qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "Unknown elements type " << type;
        }
    }
}

QList<AutoGenerateInteractionUiViewBlockBase::StateInfo> AutoGenerateInteractionUiViewActionsBlock::serializeBlockState() const
{
    QList<AutoGenerateInteractionUiViewBlockBase::StateInfo> lst;
    for (const auto &e : std::as_const(mElements)) {
        lst.append({QString::fromLatin1(e->actionId()), e->currentValue()});
    }
    return lst;
}

QList<AutoGenerateInteractionUiViewActionable *> AutoGenerateInteractionUiViewActionsBlock::elements() const
{
    return mElements;
}

void AutoGenerateInteractionUiViewActionsBlock::setElements(const QList<AutoGenerateInteractionUiViewActionable *> &newElements)
{
    mElements = newElements;
}

QWidget *AutoGenerateInteractionUiViewActionsBlock::generateWidget(QWidget *parent)
{
    auto widget = new QWidget(parent);
    parent->layout()->addWidget(widget);
    auto hboxLayout = new FlowLayout;
    hboxLayout->setContentsMargins({});
    widget->setLayout(hboxLayout);
    for (const auto &e : std::as_const(mElements)) {
        connect(e, &AutoGenerateInteractionUiViewActionable::actionChanged, this, [this](const QByteArray &actionId, const QString &value) {
            // qDebug() << "AutoGenerateInteractionUiViewActionsBlock: actionId " << actionId << " value " << value << "blockId " << mBlockId;
            Q_EMIT actionChanged(mBlockId, actionId, value);
        });
        hboxLayout->addWidget(e->generateWidget(parent));
    }
    return widget;
}

void AutoGenerateInteractionUiViewActionsBlock::setErrorMessages(const QMap<QString, QString> &map)
{
    for (const auto &e : std::as_const(mElements)) {
        e->setErrorMessages(map);
    }
}

void AutoGenerateInteractionUiViewActionsBlock::assignState(const QList<StateInfo> &info)
{
    for (const auto &e : std::as_const(mElements)) {
        for (const auto &stateInfo : info) {
            if (QString::fromLatin1(e->actionId()) == stateInfo.actionId) {
                e->setCurrentValue(stateInfo.value);
            }
        }
    }
}

void AutoGenerateInteractionUiViewActionsBlock::serializeBlock(QJsonObject &o) const
{
    QJsonArray array;
    for (const auto &e : mElements) {
        array.append(e->serialize());
    }
    o["elements"_L1] = array;
}

#include "moc_autogenerateinteractionuiviewactionsblock.cpp"
