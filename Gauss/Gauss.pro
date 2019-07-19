TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

HEADERS += \
    ExtMatrix3D/ExtMatrix3D.h \
    Exeption.h \
    Vector3D.h \
    Matrix3D/SysLinearEquat3D.h \
    Segment3D.h \
    Matrix3D/Matrix3D.h \
    ExtMatrix3D/SysLinearEquat3D.h
