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
#include <vtkVectorText.h>
#include <vtkTextProperty.h>
#include <sstream>
#include <string>


// anonymous namespace prevents pollution of global namespace in other TUs
namespace {
class PickAnnotater : public vtkCommand {


public:
  
  static PickAnnotater * New() {return new PickAnnotater;}

  vtkSmartPointer<vtkVectorText> txt;
  vtkSmartPointer<vtkPolyDataMapper> txtMapper;
  vtkSmartPointer<vtkActor> txtActor;

  std::string coordString;


  PickAnnotater() {
    txt = vtkVectorText::New();
    txt->SetText("peup");
    txtMapper = vtkPolyDataMapper::New();
    txtMapper->SetInputConnection(txt->GetOutputPort());
    txtActor = vtkActor::New();
    txtActor->SetMapper(txtMapper);
    txtActor->SetScale(0.3,0.3,0.3);
    txtActor->VisibilityOff();
  }

  void SetColor(double * colorData) {
    txtActor->GetProperty()->SetColor(colorData);
  }

  virtual void Execute(vtkObject * caller, unsigned long, void*) override {
    vtkPropPicker * picker = vtkPropPicker::SafeDownCast(caller);
    if (picker) {
      double pickPos[3];
      picker->GetPickPosition(pickPos);
      std::ostringstream oss;
      oss << "(" << pickPos[0] << ", " << pickPos[1] << ", " << pickPos[2] << ")";
      coordString = oss.str();
      txtActor->SetPosition(pickPos);
      txtActor->VisibilityOn();
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
    pickAnnotater->SetColor(colors->GetColor3d("Tomato").GetData());

    vtkNew<vtkRenderer> ren;
    ren->AddActor(thingActor);
    ren->AddActor(pickAnnotater->txtActor);
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
