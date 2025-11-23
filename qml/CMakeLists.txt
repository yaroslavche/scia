set(TARGET_NAME Qml)

set(QML_IMPORT_PATH ${CMAKE_BINARY_DIR}/qml CACHE STRING "" FORCE)
set(QML_FILES
        Layout/Main.qml
        Layout/TopPanel.qml
        Layout/ContentArea.qml

        Component/PageLoader.qml
        Component/Page.qml
        Component/UIMessageOverlay.qml
        Component/UIToast.qml

        Page/Home.qml

        Page/Person/PersonList.qml
        Page/Person/Person.qml
)

foreach (qml ${QML_FILES})
    set_property(SOURCE ${qml} PROPERTY QT_RESOURCE_ALIAS ${qml})
endforeach ()

qt_add_qml_module(${TARGET_NAME}
        URI ${PROJECT_NAME}
        VERSION 1.0
        RESOURCE_PREFIX /qt/qml
        QML_FILES ${QML_FILES}
        OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}"
)

add_library(${PROJECT_NAME}::Qml ALIAS ${TARGET_NAME})
