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
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleSwitch.h>
#include <vtkPropPicker.h>


// anonymous namespace prevents pollution of global namespace in other TUs
namespace {
class DisplayPick : public vtkCommand {
public:
  static DisplayPick * New() {return new DisplayPick;}

  virtual void Execute(vtkObject * caller, unsigned long, void*) override {
    vtkPropPicker * picker = vtkPropPicker::SafeDownCast(caller);
    if (picker) {
      double pickPos[3];
      picker->GetPickPosition(pickPos);
      std::cout << "pick! location: " << pickPos[0] << ", " << pickPos[1] << ", " << pickPos[2] << std::endl;
    }
  }


};
} // anonymous namespace



int main() {

    vtkNew<vtkNamedColors> colors;

    vtkNew<vtkSTLReader> thingReader;
    thingReader->SetFileName("../thing.stl");


    vtkNew<vtkPolyDataMapper> thingMapper;
    thingMapper->SetInputConnection(thingReader->GetOutputPort());

    vtkNew<vtkActor> thingActor;
    thingActor->SetMapper(thingMapper);
    
    vtkProperty * thingProperty = thingActor->GetProperty();
    thingProperty->SetColor(colors->GetColor3d("MistyRose").GetData());

    vtkNew<vtkRenderer> ren;
    ren->AddActor(thingActor);
    ren->SetBackground(colors->GetColor3d("MidnightBlue").GetData());

    vtkNew<vtkRenderWindow> renWin;
    renWin->AddRenderer(ren);
    renWin->SetSize(600,600);
    renWin->SetWindowName("thing");

    vtkNew<vtkRenderWindowInteractor> interactor;
    renWin->SetInteractor(interactor);

    // Set the initial interactor style to my favorite one
    auto iStyle = vtkInteractorStyleSwitch::SafeDownCast(interactor->GetInteractorStyle());
    if (iStyle)
        iStyle->SetCurrentStyleToTrackballCamera();
    else
        throw std::runtime_error("Failed o downcast to a vtkInteractorStyleSwitch!");
    

    // Press 'p' to trigger interactor->GetPicker()->Pick(...) at the mouse location
    vtkNew<DisplayPick> displayPick;
    interactor->GetPicker()->AddObserver(vtkCommand::EndPickEvent, displayPick);

    // Note that you can set the picker to be whatever subclass of vtkAbstractPicker you want
    // However it seems that you would then have to customize the interactor style completely
    // by subclassing, say, vtkInteractorStyleTrackballCamera, or a more base class.
    // In your custom interactor syle you would have to put in the call to the picker's Pick method.
    // IDK why, but when I tried setting the interactor picker to some other picker
    // without writing a custom interactor style, the Pick method was no longer triggered when I pressed 'p'



    renWin->Render();


    interactor->Start();
    


}
