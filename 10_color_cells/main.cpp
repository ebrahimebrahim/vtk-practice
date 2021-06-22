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
#include <vtkCellData.h>
#include <vtkFloatArray.h>
#include <vtkLookupTable.h>

#include <iostream>


namespace{

class BoxWidgetCommand : public vtkCommand {
public:

    static BoxWidgetCommand * New() {return new BoxWidgetCommand;}

    void Execute(vtkObject* caller, unsigned long, void*) {
        vtkNew<vtkTransform> t;
        auto boxWidget = vtkBoxWidget::SafeDownCast(caller);
        if (!boxWidget) return;
        boxWidget->GetTransform(t);
        boxWidget->GetProp3D()->SetUserTransform(t);
    }

};

} // anon namespace



int main() {

    vtkNew<vtkNamedColors> colors;


    vtkNew<vtkConeSource> cone;
    cone->SetHeight(3.0);
    cone->SetRadius(1.0);
    cone->SetResolution(50);

    vtkNew<vtkPolyDataMapper> coneMapper;
    coneMapper->SetInputConnection(cone->GetOutputPort());

    cone->Update(); // I think this is needed because of VTK's laziness
    vtkPolyData* coneData = cone->GetOutput(); // coneData is a special type of vtkDataSet
    auto numCells = coneData->GetNumberOfCells();
    vtkNew<vtkFloatArray> coneCellScalars;
    coneCellScalars->SetNumberOfTuples(numCells); // allocates memoery in the vtkFloatArray
    float stepSize = 2.0 / numCells;
    for (int i=0; i<numCells; ++i) {
        // Using SetTuple instead of InsertTuple means I've already allocated the memory-- it would segfault if i hadn't
        coneCellScalars->SetTuple1(i,0.0 + stepSize*i); 
    }

    vtkCellData * coneCellData  = coneData->GetCellData(); // coneCellData is one of the vtkDataSetAttributes of the vtkDataSet coneData
    // Here we set the vtkFloatArray that we made to be the scalar array for the vtkDataSetAttributes coneCellData
    coneCellData->SetScalars(coneCellScalars);

    // Next I'd like to make a color lookup table to more finely control the color choices of the cells. I don't even know why they are colored right now.
    vtkNew<vtkLookupTable> lut;
    lut->SetNumberOfTableValues(numCells);
    lut->SetTableRange(0.0,2.0);
    lut->SetHueRange(0.0,0.3);
    lut->SetValueRange(0.7,0.4);
    lut->SetSaturationRange(0.5,0.5);
    lut->Build();
    lut->SetTableValue(0,colors->GetColor4d("orchid_dark").GetData()); // choosing the color of the cell for the base of the cone
    coneMapper->SetLookupTable(lut);
    coneMapper->SetScalarRange(0.0,2.0);

    std::cout << numCells << " " << coneCellScalars->GetTuple1(numCells-1) <<std::endl;



    vtkNew<vtkActor> coneActor;
    coneActor->SetMapper(coneMapper);
    coneActor->GetProperty()->SetColor(colors->GetColor3d("Bisque").GetData()); // This seems to get ignored when there are scalars attached to the cone's cells.

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

    vtkNew<BoxWidgetCommand> boxWidgetCommand;
    boxWidget->AddObserver(vtkCommand::InteractionEvent,boxWidgetCommand);
    

    // press "i" to turn it on
    // boxWidget->On();

    i->Start();




    return 0;
}