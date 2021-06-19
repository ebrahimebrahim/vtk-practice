#include <vtkActor.h>
#include <vtkBoxWidget.h>
#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkConeSource.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkTransform.h>

int main() {

    vtkNew<vtkNamedColors> colors;


    vtkNew<vtkConeSource> cone;
    cone->SetHeight(3.0);
    cone->SetRadius(1.0);
    cone->SetResolution(10);

    vtkNew<vtkPolyDataMapper> coneMapper;
    coneMapper->SetInputConnection(cone->GetOutputPort());

    vtkNew<vtkActor> coneActor;
    coneActor->SetMapper(coneMapper);
    coneActor->GetProperty()->SetColor(colors->GetColor3d("Bisque").GetData());

    vtkNew<vtkRenderer> r;
    r->AddActor(coneActor);
    r->SetBackground(colors->GetColor3d("MidnightBlue").GetData());

    vtkNew<vtkRenderWindow> w;
    w->AddRenderer(r);
    w->SetSize(600,600);
    w->SetWindowName("Tut Step 6: 3D Widgets");

    vtkNew<vtkRenderWindowInteractor> i;
    i->SetRenderWindow(w);
    vtkNew<vtkInteractorStyleTrackballCamera> style;
    i->SetInteractorStyle(style);


    vtkNew<vtkBoxWidget> boxWidget;
    boxWidget->SetInteractor(i); // sets a bunch of callbacks!
    // initial size of widget as factor of bounding box of input vtkProp3D:
    boxWidget->SetPlaceFactor(1.25); 
    boxWidget->GetOutlineProperty()->SetColor(colors->GetColor3d("Gold").GetData());

    boxWidget->SetProp3D(coneActor); // here the cone will be inpit to the widget
    boxWidget->PlaceWidget(); // note that we don't add it as an actor to r
    // (instead, it presumably gets in via coneActor and maybe also via i)
    

    boxWidget->On();

    // i->Initialize();
    i->Start();




    return 0;
}