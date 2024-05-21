QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    global_data.cpp \
    main.cpp \
    mainwindow.cpp \
    promotionwindow.cpp \
    solver.cpp

HEADERS += \
    global_data.h \
    mainwindow.h \
    promotionwindow.h \
    solver.h

FORMS += \
    mainwindow.ui \
    promotionwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    board.png \
    chess.pro.user \
    graphics/bishopB.png \
    graphics/bishopW.png \
    graphics/board.png \
    graphics/kingB.png \
    graphics/kingW.png \
    graphics/knightB.png \
    graphics/knightW.png \
    graphics/pawnB.png \
    graphics/pawnW.png \
    graphics/queenB.png \
    graphics/queenW.png \
    graphics/rookB.png \
    graphics/rookW.png \
    graphics/splash.jpg

RESOURCES += \
    resources.qrc
