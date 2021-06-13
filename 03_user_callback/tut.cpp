#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkConeSource.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCommand.h>
#include <iostream>

// anonymous namespace prevents pollution of global namespace in other TUs
namespace {
class customCallback : public vtkCommand {
public:
  static customCallback * New() {return new customCallback;}
  int n = 1;

  virtual void Execute(vtkObject * caller, unsigned long, void*) override {
    vtkRenderer * ren = vtkRenderer::SafeDownCast(caller);
    if (ren) {
      ren->SetBackground(0.9,0.7,0.7);
      std::cout << "Custom callback has been called for the " << n << "th time!\n";
      std::cout << ren->GetActiveCamera()->GetPosition()[0] << " "
                << ren->GetActiveCamera()->GetPosition()[1] << " "
                << ren->GetActiveCamera()->GetPosition()[2] << std::endl;
      ++n;
    }
  }


};
} // anonymous namespace

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
    vtkNew<customCallback> cc;
    ren->AddObserver(vtkCommand::StartEvent, cc);

    vtkNew<vtkRenderWindow> renWin;
    renWin->AddRenderer(ren);
    renWin->SetSize(300,300);
    renWin->SetWindowName("tut_step2");

    // I had to comment out the following line.
    // For some reason, calling GetActiveCamera on the renderer
    // without following it by "->Azimuth" seems to
    // reposition the camera. Not understanding why.
    // vtkCamera * camera = ren->GetActiveCamera();

    for (int i=0; i<360; ++i) {
        renWin->Render();
        // The call to vtkRenderWindow::Render seems to involve
        // waiting for a frame's worth of time.
        // Seems like 60 FPS, like vsync is being used.
        ren->GetActiveCamera()->Azimuth(1);
    }


}
