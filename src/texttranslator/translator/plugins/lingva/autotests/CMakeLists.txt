# SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
ecm_add_test(lingvaengineclienttest.cpp lingvaengineclienttest.h ../lingvaengineclient.cpp ../lingvaengineplugin.cpp ../lingvaengineutil.cpp
    ../lingvaenginedialog.cpp ../lingvaenginewidget.cpp ${translator_lingva_debug_SRCS}
    TEST_NAME lingvaengineclienttest
    
    LINK_LIBRARIES Qt::Test Qt::Gui KF5::TextTranslator KF5::I18n KF5::ConfigCore
)

#######
ecm_add_test(lingvaenginewidgettest.cpp lingvaenginewidgettest.h ../lingvaenginewidget.cpp
    TEST_NAME lingvaenginewidgettest
    
    LINK_LIBRARIES Qt::Test Qt::Gui KF5::TextTranslator KF5::I18n KF5::ConfigCore
)

#######
ecm_add_test(lingvaenginedialogtest.cpp lingvaenginedialogtest.h ../lingvaenginedialog.cpp ../lingvaenginewidget.cpp
    TEST_NAME lingvaenginedialogtest
    
    LINK_LIBRARIES Qt::Test Qt::Gui KF5::TextTranslator KF5::I18n KF5::ConfigCore
)

#######
ecm_add_test(lingvaengineutiltest.h lingvaengineutiltest.cpp ../lingvaengineutil.cpp
    TEST_NAME lingvaengineutiltest
    
    LINK_LIBRARIES Qt::Test Qt::Gui KF5::TextTranslator KF5::I18n KF5::ConfigCore
)
