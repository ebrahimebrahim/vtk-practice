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
#include <vtkPlatonicSolidSource.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleSwitch.h>
#include <vtkPropPicker.h>
#include <vtkColorTransferFunction.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <sstream>
#include <string>


// anonymous namespace prevents pollution of global namespace in other TUs
namespace {
class PickAnnotater : public vtkCommand {


public:
  
  static PickAnnotater * New() {return new PickAnnotater;}

  vtkSmartPointer<vtkTextActor> txt;
  std::string txtString;

  PickAnnotater() {
    txt = vtkTextActor::New();
    txt->SetInput("peup");
    vtkTextProperty * txtProperty = txt->GetTextProperty();
    txtProperty->SetFontFamilyToCourier();
    txtProperty->SetFontSize(12);
    txtProperty->ShadowOn();
    txt->SetDisplayPosition(300,300);
    txt->VisibilityOff();
  }

  void SetColor(double * colorData) {
    txt->GetTextProperty()->SetColor(colorData);
  }

  virtual void Execute(vtkObject * caller, unsigned long, void*) override {
    vtkPropPicker * picker = vtkPropPicker::SafeDownCast(caller);
    if (picker) {
      double pickPos[3];
      picker->GetPickPosition(pickPos);
      std::ostringstream oss;
      oss << "(" << pickPos[0] << ", " << pickPos[1] << ", " << pickPos[2] << ")";
      txtString = oss.str();
      txt->SetInput(txtString.c_str());
      txt->SetDisplayPosition(250,300);
      txt->VisibilityOn();
    }
  }


};
} // anonymous namespace



int main() {

    vtkNew<vtkNamedColors> colors;

    vtkNew<vtkPlatonicSolidSource> solid;
    solid->SetSolidType(4); // 4 is the docdecahedron

    vtkNew<vtkColorTransferFunction> colorFunction;
    colorFunction->AddRGBSegment(0.0,1.0, 0.0, 0.0, 11, 0.0, 1.0, 0.0);

    vtkNew<vtkPolyDataMapper> thingMapper;
    thingMapper->SetInputConnection(solid->GetOutputPort());
    thingMapper->SetLookupTable(colorFunction);
    thingMapper->SetScalarRange(0,11);

    vtkNew<vtkActor> thingActor;
    thingActor->SetMapper(thingMapper);
    
    vtkProperty * thingProperty = thingActor->GetProperty();

    vtkNew<PickAnnotater> pickAnnotater;
    pickAnnotater->SetColor(colors->GetColor3d("CornSilk").GetData());

    vtkNew<vtkRenderer> ren;
    ren->AddActor(thingActor);
    ren->AddActor(pickAnnotater->txt);
    ren->SetBackground(colors->GetColor3d("MidnightBlue").GetData());

    vtkNew<vtkRenderWindow> renWin;
    renWin->AddRenderer(ren);
    renWin->SetSize(600,600);
    renWin->SetWindowName("annotate");

    vtkNew<vtkRenderWindowInteractor> interactor;
    renWin->SetInteractor(interactor);

    // Set the initial interactor style to my favorite one
    auto iStyle = vtkInteractorStyleSwitch::SafeDownCast(interactor->GetInteractorStyle());
    if (iStyle)
        iStyle->SetCurrentStyleToTrackballCamera();
    else
        throw std::runtime_error("Failed o downcast to a vtkInteractorStyleSwitch!");

    // Press 'p' to trigger interactor->GetPicker()->Pick(...) at the mouse location
    interactor->GetPicker()->AddObserver(vtkCommand::EndPickEvent, pickAnnotater);
    // Hmm I just noticed that if you don't move the camera at all after the interactor starts
    // and you press 'p' for a pick, there's no pick and there's a WARN in stdout:
    // WARN| vtkInteractorStyleTrackballCamera (0x559a043af800): no current renderer on the interactor style.
    // I wonder why this is. It seems you need to interact with the interactor for picking to work.

    



    renWin->Render();


    interactor->Start();
    


}
