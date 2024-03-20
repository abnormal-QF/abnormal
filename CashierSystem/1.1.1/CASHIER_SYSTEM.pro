QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(src/my_barcode/my_barcode.pri)

INCLUDEPATH += $$PWD/src/my_barcode


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    my_captcha.cpp \
    my_database.cpp \
    my_login.cpp \
    my_register.cpp

HEADERS += \
    mainwindow.h \
    my_captcha.h \
    my_database.h \
    my_login.h \
    my_register.h

FORMS += \
    mainwindow.ui \
    my_captcha.ui \
    my_login.ui \
    my_register.ui

MOC_DIR = temp/moc
RCC_DIR = temp/rccr
OBJECTS_DIR = temp/obj
DESTDIR = bin

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += D:\QT\OpenCV\opencv\newBuild\install\include
LIBS += D:\QT\OpenCV\opencv\newBuild\lib\libopencv_*.a


#使用qzxing需要添加内容
CONFIG += enable_encoder_qr_code
LIBS += -L$$PWD/lib  -lQZXing3   #添加库
DEFINES += ENABLE_ENCODER_GENERIC   #开启二维码生成功能


RESOURCES += \
    resource.qrc
