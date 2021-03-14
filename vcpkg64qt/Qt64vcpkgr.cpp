#include "Qt64vcpkgr.h"

#include <vtk-9.0/vtkCamera.h>
#include <vtk-9.0/vtkCubeSource.h>
#include <vtk-9.0/vtkDataObjectToTable.h>
#include <vtk-9.0/vtkElevationFilter.h>
#include <vtk-9.0/vtkGenericOpenGLRenderWindow.h>
#include <vtk-9.0/vtkNamedColors.h>
#include <vtk-9.0/vtkNew.h>
#include <vtk-9.0/vtkPolyDataMapper.h>
#include <vtk-9.0/vtkQtTableView.h>
#include <vtk-9.0/vtkRenderWindow.h>
#include <vtk-9.0/vtkRenderer.h>
#include <vtk-9.0/vtkSmartPointer.h>
#include <vtk-9.0/vtkSphereSource.h>
#include <vtk-9.0/vtkVersion.h>

#if VTK_VERSION_NUMBER >= 89000000000ULL
#define VTK890 1
#endif

Qt64vcpkgr::Qt64vcpkgr(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);

	vtkNew<vtkNamedColors> colors;

	vtkNew<vtkGenericOpenGLRenderWindow> renderWindowLeft;
	vtkNew<vtkGenericOpenGLRenderWindow> renderWindowRight;
#if VTK890
	this->qvtkWidgetLeft->setRenderWindow(renderWindowLeft);
	this->qvtkWidgetRight->setRenderWindow(renderWindowRight);
#else
	this->qvtkWidgetLeft->SetRenderWindow(renderWindowLeft);
	this->qvtkWidgetRight->SetRenderWindow(renderWindowRight);
#endif

	// Sphere
	auto sphereSource = vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->SetPhiResolution(30);
	sphereSource->SetThetaResolution(30);
	sphereSource->Update();
	vtkNew<vtkElevationFilter> sphereElev;
	sphereElev->SetInputConnection(sphereSource->GetOutputPort());
	sphereElev->SetLowPoint(0, -1.0, 0);
	sphereElev->SetHighPoint(0, 1.0, 0);
	auto sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	sphereMapper->SetInputConnection(sphereElev->GetOutputPort());
	auto sphereActor = vtkSmartPointer<vtkActor>::New();
	sphereActor->SetMapper(sphereMapper);

	// Cube
	auto cubeSource = vtkSmartPointer<vtkCubeSource>::New();
	cubeSource->Update();
	vtkNew<vtkElevationFilter> cubeElev;
	cubeElev->SetInputConnection(cubeSource->GetOutputPort());
	cubeElev->SetLowPoint(0, -1.0, 0);
	cubeElev->SetHighPoint(0, 1.0, 0);
	auto cubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	cubeMapper->SetInputConnection(cubeElev->GetOutputPort());
	auto cubeActor = vtkSmartPointer<vtkActor>::New();
	cubeActor->SetMapper(cubeMapper);

	// VTK Renderer
	auto leftRenderer = vtkSmartPointer<vtkRenderer>::New();
	leftRenderer->AddActor(sphereActor);
	leftRenderer->SetBackground(colors->GetColor3d("LightSteelBlue").GetData());

	auto rightRenderer = vtkSmartPointer<vtkRenderer>::New();

	// Add Actor to renderer
	rightRenderer->AddActor(cubeActor);
	rightRenderer->GetActiveCamera()->SetPosition(1.0, 0.8, 1.0);
	rightRenderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	rightRenderer->SetBackground(colors->GetColor3d("LightSteelBlue").GetData());
	rightRenderer->ResetCamera();
	rightRenderer->GetActiveCamera()->Zoom(0.8);

	// VTK/Qt wedded
#if VTK890
	this->qvtkWidgetLeft->renderWindow()->AddRenderer(leftRenderer);
	this->qvtkWidgetRight->renderWindow()->AddRenderer(rightRenderer);
#else
	this->qvtkWidgetLeft->GetRenderWindow()->AddRenderer(leftRenderer);
	this->qvtkWidgetRight->GetRenderWindow()->AddRenderer(rightRenderer);
#endif

	// Set up action signals and slots
	connect(this->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
}

void Qt64vcpkgr::slotExit()
{
	qApp->exit();
}
