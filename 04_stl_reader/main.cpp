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
#include <vtkSTLReader.h>

// anonymous namespace prevents pollution of global namespace in other TUs
namespace {
class customCallback : public vtkCommand {
public:
  static customCallback * New() {return new customCallback;}

  virtual void Execute(vtkObject * caller, unsigned long, void*) override {
    vtkRenderer * ren = vtkRenderer::SafeDownCast(caller);
    if (ren) {
      std::cout << ren->GetActiveCamera()->GetPosition()[0] << " "
                << ren->GetActiveCamera()->GetPosition()[1] << " "
                << ren->GetActiveCamera()->GetPosition()[2] << std::endl;
    }
  }


};
} // anonymous namespace

int main() {

    vtkNew<vtkNamedColors> colors;

    vtkNew<vtkSTLReader> thingReader;
    thingReader->SetFileName("../thing.stl");

    // This is a mapper, so it ends the "visualization pipeline".
    // This will map polygonal data into graphics primitives.
    vtkNew<vtkPolyDataMapper> thingMapper;
    thingMapper->SetInputConnection(thingReader->GetOutputPort());

    vtkNew<vtkActor> thingActor;
    thingActor->SetMapper(thingMapper);
    
    // Making an actor creates a vtkProperty to go with it
    // If we wanted to, we could share this property object with other actors
    vtkProperty * thingProperty = thingActor->GetProperty();
    thingProperty->SetColor(colors->GetColor3d("MistyRose").GetData());

    vtkNew<vtkRenderer> ren;
    ren->AddActor(thingActor);
    ren->SetBackground(colors->GetColor3d("MidnightBlue").GetData());
    vtkNew<customCallback> cc;
    ren->AddObserver(vtkCommand::StartEvent, cc);

    vtkNew<vtkRenderWindow> renWin;
    renWin->AddRenderer(ren);
    renWin->SetSize(300,300);
    renWin->SetWindowName("rotating thing");

    for (int i=0; i<360; ++i) {
        renWin->Render();
        ren->GetActiveCamera()->Azimuth(1);
    }


}
