QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    albumcollection.cpp \
    createalbumdialog.cpp \
    databasemanager.cpp \
    databaseviewer.cpp \
    deletealbumdialog.cpp \
    filemanager.cpp \
    main.cpp \
    mainwindow.cpp \
    photocollection.cpp \
    photoeditor.cpp

HEADERS += \
    albumcollection.h \
    createalbumdialog.h \
    databasemanager.h \
    databaseviewer.h \
    deletealbumdialog.h \
    filemanager.h \
    mainwindow.h \
    photocollection.h \
    photoeditor.h \
    photoinfo.h \
    ui_createalbumdialog.h

FORMS += \
    albumcollection.ui \
    createalbumdialog.ui \
    databaseviewer.ui \
    deletealbumdialog.ui \
    filemanager.ui \
    mainwindow.ui \
    photocollection.ui \
    photoeditor.ui

RC_ICONS = ./icon/album.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    db.qrc \
    icon.qrc \
    img.qrc \
    qss.qrc

