/****************************************************************************
** Meta object code from reading C++ file 'shadercontrols.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../4_OpenGL_Fun/src/shadercontrols.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'shadercontrols.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ShaderControls_t {
    QByteArrayData data[10];
    char stringdata0[264];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ShaderControls_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ShaderControls_t qt_meta_stringdata_ShaderControls = {
    {
QT_MOC_LITERAL(0, 0, 14), // "ShaderControls"
QT_MOC_LITERAL(1, 15, 19), // "sig_setCurrentModel"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 34), // "sig_setCurrentSurfaceShaderPr..."
QT_MOC_LITERAL(4, 71, 38), // "sig_setCurrentPostprocessShad..."
QT_MOC_LITERAL(5, 110, 27), // "sig_setCurrentMatcapTexture"
QT_MOC_LITERAL(6, 138, 20), // "slot_setCurrentModel"
QT_MOC_LITERAL(7, 159, 35), // "slot_setCurrentSurfaceShaderP..."
QT_MOC_LITERAL(8, 195, 39), // "slot_setCurrentPostprocessSha..."
QT_MOC_LITERAL(9, 235, 28) // "slot_setCurrentMatcapTexture"

    },
    "ShaderControls\0sig_setCurrentModel\0\0"
    "sig_setCurrentSurfaceShaderProgram\0"
    "sig_setCurrentPostprocessShaderProgram\0"
    "sig_setCurrentMatcapTexture\0"
    "slot_setCurrentModel\0"
    "slot_setCurrentSurfaceShaderProgram\0"
    "slot_setCurrentPostprocessShaderProgram\0"
    "slot_setCurrentMatcapTexture"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ShaderControls[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       3,    1,   57,    2, 0x06 /* Public */,
       4,    1,   60,    2, 0x06 /* Public */,
       5,    1,   63,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   66,    2, 0x0a /* Public */,
       7,    1,   69,    2, 0x0a /* Public */,
       8,    1,   72,    2, 0x0a /* Public */,
       9,    1,   75,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void ShaderControls::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ShaderControls *_t = static_cast<ShaderControls *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sig_setCurrentModel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->sig_setCurrentSurfaceShaderProgram((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->sig_setCurrentPostprocessShaderProgram((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->sig_setCurrentMatcapTexture((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->slot_setCurrentModel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->slot_setCurrentSurfaceShaderProgram((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->slot_setCurrentPostprocessShaderProgram((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->slot_setCurrentMatcapTexture((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ShaderControls::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ShaderControls::sig_setCurrentModel)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ShaderControls::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ShaderControls::sig_setCurrentSurfaceShaderProgram)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (ShaderControls::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ShaderControls::sig_setCurrentPostprocessShaderProgram)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (ShaderControls::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ShaderControls::sig_setCurrentMatcapTexture)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject ShaderControls::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ShaderControls.data,
      qt_meta_data_ShaderControls,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ShaderControls::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ShaderControls::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ShaderControls.stringdata0))
        return static_cast<void*>(const_cast< ShaderControls*>(this));
    return QWidget::qt_metacast(_clname);
}

int ShaderControls::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void ShaderControls::sig_setCurrentModel(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ShaderControls::sig_setCurrentSurfaceShaderProgram(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ShaderControls::sig_setCurrentPostprocessShaderProgram(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ShaderControls::sig_setCurrentMatcapTexture(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
