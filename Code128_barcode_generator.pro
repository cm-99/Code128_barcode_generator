QT       += core gui
QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    GUI/barcodesettingsgui.cpp \
    GUI/charactervariablebarcodepartgui.cpp \
    GUI/expandablewidget.cpp \
    GUI/filegenerationsettingsgui.cpp \
    GUI/fixedbarcodepartgui.cpp \
    GUI/numbervariablebarcodepartgui.cpp \
    barcodesettingsmanager.cpp \
    code128barcodeitem.cpp \
    code128encoder.cpp \
    filegenerationsettingsmanager.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    GUI/barcodesettingsgui.h \
    GUI/charactervariablebarcodepartgui.h \
    GUI/expandablewidget.h \
    GUI/filegenerationsettingsgui.h \
    GUI/fixedbarcodepartgui.h \
    GUI/numbervariablebarcodepartgui.h \
    barcodeencoder.h \
    barcodeitem.h \
    barcodesettingsmanager.h \
    code128barcodeitem.h \
    code128encoder.h \
    filegenerationsettingsmanager.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

