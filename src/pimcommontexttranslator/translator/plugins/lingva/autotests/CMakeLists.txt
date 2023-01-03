# SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
ecm_add_test(lingvaengineclienttest.cpp lingvaengineclienttest.h ../lingvaengineclient.cpp ../lingvaengineplugin.cpp ../lingvaengineutil.cpp
    ../lingvaenginedialog.cpp ../lingvaenginewidget.cpp ${translator_lingva_debug_SRCS}
    TEST_NAME lingvaengineclienttest
    NAME_PREFIX pimcommon-
    LINK_LIBRARIES Qt::Test Qt::Gui KF5::PimCommonTextTranslator KF5::I18n KF5::ConfigCore
)

#######
ecm_add_test(lingvaenginewidgettest.cpp lingvaenginewidgettest.h ../lingvaenginewidget.cpp
    TEST_NAME lingvaenginewidgettest
    NAME_PREFIX pimcommon-
    LINK_LIBRARIES Qt::Test Qt::Gui KF5::PimCommonTextTranslator KF5::I18n KF5::ConfigCore
)

#######
ecm_add_test(lingvaenginedialogtest.cpp lingvaenginedialogtest.h ../lingvaenginedialog.cpp ../lingvaenginewidget.cpp
    TEST_NAME lingvaenginedialogtest
    NAME_PREFIX pimcommon-
    LINK_LIBRARIES Qt::Test Qt::Gui KF5::PimCommonTextTranslator KF5::I18n KF5::ConfigCore
)

#######
ecm_add_test(lingvaengineutiltest.h lingvaengineutiltest.cpp ../lingvaengineutil.cpp
    TEST_NAME lingvaengineutiltest
    NAME_PREFIX pimcommon-
    LINK_LIBRARIES Qt::Test Qt::Gui KF5::PimCommonTextTranslator KF5::I18n KF5::ConfigCore
)
