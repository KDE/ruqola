/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewsectionblock.h"
#include "autogenerateui/autogenerateinteractionuiviewtext.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewbuttonelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewdatepickerelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewimageelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewmultistaticselectelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewoverflowelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewstaticselectelement.h"
#include "autogenerateui/widgets/actionelementwidget.h"
#include "ruqola_autogenerateui_debug.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QLayout>
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewSectionBlock::AutoGenerateInteractionUiViewSectionBlock(QObject *parent)
    : AutoGenerateInteractionUiViewBlockBase(parent)
{
}

AutoGenerateInteractionUiViewSectionBlock::~AutoGenerateInteractionUiViewSectionBlock()
{
    delete mText;
    delete mAccessory;
    qDeleteAll(mFields);
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewSectionBlock &t)
{
    d.space() << "AutoGenerateInteractionUiViewBlockBase:" << static_cast<const AutoGenerateInteractionUiViewBlockBase &>(t);
    if (t.text()) {
        d.space() << "text:" << *t.text();
    }
    if (t.accessory()) {
        d.space() << "accessory:" << *t.accessory();
    }
    d.space() << "fields:" << t.fields();
    return d;
}

bool AutoGenerateInteractionUiViewSectionBlock::operator==(const AutoGenerateInteractionUiViewSectionBlock &other) const
{
    return AutoGenerateInteractionUiViewBlockBase::operator==(other) && other.text() == text() && other.accessory() == accessory()
        && other.fields() == fields();
}

QWidget *AutoGenerateInteractionUiViewSectionBlock::generateWidget(QWidget *parent)
{
    auto widget = new QWidget(parent);
    parent->layout()->addWidget(widget);
    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setContentsMargins({});
    widget->setLayout(hboxLayout);

    if (mText && !mText->isEmpty()) {
        auto label = new QLabel(parent);
        label->setWordWrap(true);
        label->setOpenExternalLinks(true);
        label->setText(mText->generateText());
        hboxLayout->addWidget(label);
    }
    if (mAccessory) {
        auto w = mAccessory->generateWidget(parent);
        connect(mAccessory, &AutoGenerateInteractionUiViewActionable::actionChanged, this, [this](const QByteArray &actionId, const QString &value) {
            // qDebug() << "AutoGenerateInteractionUiViewActionsBlock: actionId " << actionId << " value " << value << "blockId " << mBlockId;
            Q_EMIT actionChanged(mBlockId, actionId, value);
        });
        hboxLayout->addWidget(w);
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

void AutoGenerateInteractionUiViewSectionBlock::parseBlock(const QJsonObject &json)
{
    if (json.contains("text"_L1)) {
        mText = new AutoGenerateInteractionUiViewText;
        mText->parse(json["text"_L1].toObject());
    }
    if (json.contains("accessory"_L1)) {
        const QJsonObject accessoryObj = json["accessory"_L1].toObject();
        const QString type = accessoryObj["type"_L1].toString();
        if (type == "button"_L1) {
            auto b = new AutoGenerateInteractionUiViewButtonElement;
            b->parse(accessoryObj);
            mAccessory = b;
        } else if (type == "datepicker"_L1) {
            auto datePicker = new AutoGenerateInteractionUiViewDatePickerElement;
            datePicker->parse(accessoryObj);
            mAccessory = datePicker;
        } else if (type == "image"_L1) {
            auto img = new AutoGenerateInteractionUiViewImageElement;
            img->parse(accessoryObj);
            mAccessory = img;
        } else if (type == "overflow"_L1) {
            auto flow = new AutoGenerateInteractionUiViewOverflowElement;
            flow->parse(accessoryObj);
            mAccessory = flow;
        } else if (type == "static_select"_L1) {
            auto select = new AutoGenerateInteractionUiViewStaticSelectElement;
            select->parse(accessoryObj);
            mAccessory = select;
        } else if (type == "multi_static_select"_L1) {
            auto select = new AutoGenerateInteractionUiViewMultiStaticSelectElement;
            select->parse(accessoryObj);
            mAccessory = select;
        } else {
            qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "AutoGenerateInteractionUiViewSectionBlock Unknown type " << type;
        }
    }
    const auto fields = json["fields"_L1].toArray();
    for (const auto &r : fields) {
        auto e = new AutoGenerateInteractionUiViewText;
        e->parse(r.toObject());
        mFields.append(e);
    }
}

QList<AutoGenerateInteractionUiViewBlockBase::StateInfo> AutoGenerateInteractionUiViewSectionBlock::serializeBlockState() const
{
    return {};
}

QList<AutoGenerateInteractionUiViewText *> AutoGenerateInteractionUiViewSectionBlock::fields() const
{
    return mFields;
}

void AutoGenerateInteractionUiViewSectionBlock::setFields(const QList<AutoGenerateInteractionUiViewText *> &newFields)
{
    mFields = newFields;
}

void AutoGenerateInteractionUiViewSectionBlock::setErrorMessages(const QMap<QString, QString> &map)
{
    if (mAccessory) {
        mAccessory->setErrorMessages(map);
    }
}

void AutoGenerateInteractionUiViewSectionBlock::assignState(const QList<StateInfo> &info)
{
    Q_UNUSED(info);
}

AutoGenerateInteractionUiViewActionable *AutoGenerateInteractionUiViewSectionBlock::accessory() const
{
    return mAccessory;
}

void AutoGenerateInteractionUiViewSectionBlock::setAccessory(AutoGenerateInteractionUiViewActionable *newAccessory)
{
    mAccessory = newAccessory;
}

void AutoGenerateInteractionUiViewSectionBlock::serializeBlock(QJsonObject &o) const
{
    if (mText) {
        o["text"_L1] = mText->serialize();
    }
    if (mAccessory) {
        o["accessory"_L1] = mAccessory->serialize();
    }
    QJsonArray array;
    for (const auto &e : mFields) {
        array.append(e->serialize());
    }

    if (!array.isEmpty()) {
        o["fields"_L1] = array;
    }
}

#include "moc_autogenerateinteractionuiviewsectionblock.cpp"
