#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkConeSource.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSTLReader.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleSwitch.h>
#include <vtkFillHolesFilter.h>
#include <vtkMassProperties.h>
#include <vtkTriangleFilter.h>
#include <vtkPolyDataNormals.h>
#include <iostream>


int main() {

  vtkNew<vtkNamedColors> colors;

  vtkNew<vtkSTLReader> thingReader;
  thingReader->SetFileName("../thing.stl");

  // --- Compute mass ---

  // There are no holes in "thing.stl" but whatever:
  vtkNew<vtkFillHolesFilter> fillHolesFilter;
  fillHolesFilter->SetInputConnection(thingReader->GetOutputPort());

  // We get the wrong answer if we don't convert to triangles:
  vtkNew<vtkTriangleFilter> triangleFilter;
  triangleFilter->SetInputConnection(fillHolesFilter->GetOutputPort());

  vtkNew<vtkPolyDataNormals> normals;
  normals->SetInputConnection(triangleFilter->GetOutputPort());
  normals->ConsistencyOn();
  normals->SplittingOff();

  vtkNew<vtkMassProperties> massProperties;
  massProperties->SetInputConnection(normals->GetOutputPort());
  std::cout << "Mass of thing: " << massProperties->GetVolume() << std::endl;



  // --- Visualization ---

  vtkNew<vtkPolyDataMapper> thingMapper;
  thingMapper->SetInputConnection(normals->GetOutputPort());

  vtkNew<vtkActor> thingActor;
  thingActor->SetMapper(thingMapper);
  
  vtkProperty * thingProperty = thingActor->GetProperty();
  thingProperty->SetColor(colors->GetColor3d("MistyRose").GetData());
  
  // Apparently computing normals makes it look ugly with the (default?) Gouraud shading 
  thingProperty->SetInterpolationToFlat();

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
    throw std::runtime_error("Failed to downcast to a vtkInteractorStyleSwitch!");
  

  renWin->Render();


  interactor->Start();
    
  return EXIT_SUCCESS;

}
