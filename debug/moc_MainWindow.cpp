/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created: Fri Jun 26 21:57:43 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/MainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      36,   11,   11,   11, 0x08,
      44,   11,   11,   11, 0x08,
      63,   11,   11,   11, 0x08,
      87,   11,   11,   11, 0x08,
     147,   11,   11,   11, 0x08,
     181,  174,   11,   11, 0x08,
     213,  174,   11,   11, 0x08,
     247,   11,   11,   11, 0x08,
     261,   11,   11,   11, 0x08,
     295,  283,   11,   11, 0x08,
     323,  316,   11,   11, 0x0a,
     373,  352,   11,   11, 0x0a,
     410,  402,   11,   11, 0x2a,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0newTrayMessage(QString)\0"
    "about()\0newUserTextInput()\0"
    "toggleUserListVisible()\0"
    "toggleShowHideMainWindow(QSystemTrayIcon::ActivationReason)\0"
    "showConnectionWidget(bool)\0regExp\0"
    "setColorInducatorUsers(QString)\0"
    "setColorInducatorBattles(QString)\0"
    "startSpring()\0startSpringSettings()\0"
    "isBattleTab\0hideBattleList(bool)\0"
    "sample\0playSample(SampleCollection)\0"
    "message,milliseconds\0sendTrayMessage(QString,int)\0"
    "message\0sendTrayMessage(QString)\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

const QMetaObject *MainWindow::metaObject() const
{
    return &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: newTrayMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: about(); break;
        case 2: newUserTextInput(); break;
        case 3: toggleUserListVisible(); break;
        case 4: toggleShowHideMainWindow((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        case 5: showConnectionWidget((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: setColorInducatorUsers((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: setColorInducatorBattles((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: startSpring(); break;
        case 9: startSpringSettings(); break;
        case 10: hideBattleList((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: playSample((*reinterpret_cast< SampleCollection(*)>(_a[1]))); break;
        case 12: sendTrayMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 13: sendTrayMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::newTrayMessage(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
