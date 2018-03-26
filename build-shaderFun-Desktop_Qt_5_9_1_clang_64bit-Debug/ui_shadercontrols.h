/********************************************************************************
** Form generated from reading UI file 'shadercontrols.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHADERCONTROLS_H
#define UI_SHADERCONTROLS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ShaderControls
{
public:
    QGroupBox *groupBox_2;
    QComboBox *postprocessShaderComboBox;
    QLabel *label;
    QGroupBox *groupBox;
    QComboBox *surfaceShaderComboBox;
    QGroupBox *groupBox_3;
    QComboBox *modelComboBox;
    QTabWidget *tabWidget;
    QWidget *matcapTab;
    QComboBox *matcapComboBox;

    void setupUi(QWidget *ShaderControls)
    {
        if (ShaderControls->objectName().isEmpty())
            ShaderControls->setObjectName(QStringLiteral("ShaderControls"));
        ShaderControls->resize(250, 377);
        groupBox_2 = new QGroupBox(ShaderControls);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 120, 201, 61));
        QFont font;
        font.setFamily(QStringLiteral("Gadugi"));
        groupBox_2->setFont(font);
        postprocessShaderComboBox = new QComboBox(groupBox_2);
        postprocessShaderComboBox->setObjectName(QStringLiteral("postprocessShaderComboBox"));
        postprocessShaderComboBox->setGeometry(QRect(10, 30, 141, 22));
        postprocessShaderComboBox->setFont(font);
        label = new QLabel(ShaderControls);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 0, 241, 41));
        QFont font1;
        font1.setFamily(QStringLiteral("Gadugi"));
        font1.setPointSize(16);
        label->setFont(font1);
        label->setAlignment(Qt::AlignCenter);
        groupBox = new QGroupBox(ShaderControls);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 50, 201, 61));
        groupBox->setFont(font);
        surfaceShaderComboBox = new QComboBox(groupBox);
        surfaceShaderComboBox->setObjectName(QStringLiteral("surfaceShaderComboBox"));
        surfaceShaderComboBox->setGeometry(QRect(10, 30, 141, 22));
        surfaceShaderComboBox->setFont(font);
        groupBox_3 = new QGroupBox(ShaderControls);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 190, 201, 61));
        groupBox_3->setFont(font);
        modelComboBox = new QComboBox(groupBox_3);
        modelComboBox->setObjectName(QStringLiteral("modelComboBox"));
        modelComboBox->setGeometry(QRect(10, 30, 141, 22));
        modelComboBox->setFont(font);
        tabWidget = new QTabWidget(ShaderControls);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(10, 270, 201, 81));
        tabWidget->setFont(font);
        matcapTab = new QWidget();
        matcapTab->setObjectName(QStringLiteral("matcapTab"));
        matcapComboBox = new QComboBox(matcapTab);
        matcapComboBox->setObjectName(QStringLiteral("matcapComboBox"));
        matcapComboBox->setGeometry(QRect(10, 10, 141, 22));
        matcapComboBox->setFont(font);
        tabWidget->addTab(matcapTab, QString());

        retranslateUi(ShaderControls);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ShaderControls);
    } // setupUi

    void retranslateUi(QWidget *ShaderControls)
    {
        ShaderControls->setWindowTitle(QApplication::translate("ShaderControls", "Form", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("ShaderControls", "Post-Process Shader", Q_NULLPTR));
        postprocessShaderComboBox->clear();
        postprocessShaderComboBox->insertItems(0, QStringList()
         << QApplication::translate("ShaderControls", "None", Q_NULLPTR)
         << QApplication::translate("ShaderControls", "Greyscale", Q_NULLPTR)
         << QApplication::translate("ShaderControls", "Gaussian Blur", Q_NULLPTR)
         << QApplication::translate("ShaderControls", "Sobel", Q_NULLPTR)
         << QApplication::translate("ShaderControls", "Bloom", Q_NULLPTR)
         << QApplication::translate("ShaderControls", "Worley Noise", Q_NULLPTR)
        );
        label->setText(QApplication::translate("ShaderControls", "Shader Options", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("ShaderControls", "Surface Shader", Q_NULLPTR));
        surfaceShaderComboBox->clear();
        surfaceShaderComboBox->insertItems(0, QStringList()
         << QApplication::translate("ShaderControls", "Lambert", Q_NULLPTR)
         << QApplication::translate("ShaderControls", "Blinn-Phong", Q_NULLPTR)
         << QApplication::translate("ShaderControls", "Matcap", Q_NULLPTR)
         << QApplication::translate("ShaderControls", "Color Gradient", Q_NULLPTR)
         << QApplication::translate("ShaderControls", "Vertex Deformation", Q_NULLPTR)
        );
        groupBox_3->setTitle(QApplication::translate("ShaderControls", "Model", Q_NULLPTR));
        modelComboBox->clear();
        modelComboBox->insertItems(0, QStringList()
         << QApplication::translate("ShaderControls", "Wahoo", Q_NULLPTR)
         << QApplication::translate("ShaderControls", "Moo Beast", Q_NULLPTR)
         << QApplication::translate("ShaderControls", "Cube", Q_NULLPTR)
        );
        matcapComboBox->clear();
        matcapComboBox->insertItems(0, QStringList()
         << QApplication::translate("ShaderControls", "Red Clay", Q_NULLPTR)
         << QApplication::translate("ShaderControls", "Red Plastic", Q_NULLPTR)
         << QApplication::translate("ShaderControls", "Chrome", Q_NULLPTR)
         << QApplication::translate("ShaderControls", "Pearl", Q_NULLPTR)
         << QApplication::translate("ShaderControls", "Orange-Green", Q_NULLPTR)
         << QApplication::translate("ShaderControls", "Blue-Purple", Q_NULLPTR)
         << QApplication::translate("ShaderControls", "Outline", Q_NULLPTR)
         << QApplication::translate("ShaderControls", "Surface Normals", Q_NULLPTR)
        );
        tabWidget->setTabText(tabWidget->indexOf(matcapTab), QApplication::translate("ShaderControls", "Matcap Options", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ShaderControls: public Ui_ShaderControls {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHADERCONTROLS_H
