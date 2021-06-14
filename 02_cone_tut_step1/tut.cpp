#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkConeSource.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

int main() {

    vtkNew<vtkNamedColors> colors;
    // apparently vtkNew works similarly to vtkSmartPointer

    vtkNew<vtkConeSource> cone;
    // "Source" here means that we are looking at part of the 
    // visualization pipeline that requires no inputs and produces data.
    // The data it produces will be of type vtkPolyData.
    cone->SetHeight(3.0);
    cone->SetRadius(1.0);
    cone->SetResolution(10);

    // This is a mapper, so it ends the "visualization pipeline".
    // This will map polygonal data into graphics primitives.
    vtkNew<vtkPolyDataMapper> coneMapper;
    coneMapper->SetInputConnection(cone->GetOutputPort());

    vtkNew<vtkActor> coneActor;
    coneActor->SetMapper(coneMapper);
    
    // Making an actor creates a vtkProperty to go with it
    // If we wanted to, we could share this property object with other actors
    vtkProperty * coneProperty = coneActor->GetProperty();
    coneProperty->SetColor(colors->GetColor3d("MistyRose").GetData());

    vtkNew<vtkRenderer> ren;
    ren->AddActor(coneActor);
    ren->SetBackground(colors->GetColor3d("MidnightBlue").GetData());

    vtkNew<vtkRenderWindow> renWin;
    renWin->AddRenderer(ren);
    renWin->SetSize(300,300);
    renWin->SetWindowName("tut_step1");

    vtkCamera * camera = ren->GetActiveCamera();

    // This took a bit to figure out:
    // Grabbing the camera with vtkRenderer::GetActiveCamera above
    // prevents the camera from automatically getting "reset"
    // And "reset" doesn't mean what it sounds like:
    // It means move the camera along the view vector
    // (the vector from camera pos to lookAt, i.e. focus, point)
    // so that all the actors are visible to the camera.
    ren->ResetCamera();

    for (int i=0; i<360; ++i) {
        renWin->Render();
        // The call to vtkRenderWindow::Render seems to involve
        // waiting for a frame's worth of time.
        // Seems like 60 FPS, like vsync is being used.
        camera->Azimuth(2);
    }


}