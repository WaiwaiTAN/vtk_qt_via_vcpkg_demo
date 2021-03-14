#pragma once
#include <vtk-9.0/vtkSmartPointer.h>
#include <QtWidgets/QMainWindow>
#include "ui_Qt64vcpkgr.h"

class Qt64vcpkgr : public QMainWindow, private Ui::Qt64vcpkgrClass
{
    Q_OBJECT

public:
    Qt64vcpkgr(QWidget *parent = Q_NULLPTR);
	~Qt64vcpkgr() = default;

public slots:

	void slotExit();
};
