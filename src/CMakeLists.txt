set (Rucola_core_srcs
    messagemodel.cpp
    roommodel.cpp
    ddpclient.cpp
    ruqola.cpp
    rocketchatbackend.cpp
    notification.cpp
    messagequeue.cpp
    authentication.cpp
)

add_library(RucolaCore STATIC ${Rucola_core_srcs})

target_link_libraries(RucolaCore
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Qml
    Qt5::Quick
    Qt5::WebSockets
    Qt5::Network
    Qt5::NetworkAuth
)

qt5_add_resources(RuqolaResources qml/qml.qrc)

add_executable(ruqola main.cpp ${RuqolaResources})

target_link_libraries(ruqola RucolaCore)

install(TARGETS ruqola RUNTIME DESTINATION bin)
