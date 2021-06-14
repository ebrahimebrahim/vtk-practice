#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkInteractionStyle);

#include <vtkSmartPointer.h>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkTextActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleSwitch.h>
#include <vtkActor.h>
#include <stdexcept>

int main()
{
    auto cone = vtkSmartPointer<vtkConeSource>::New();
    cone->SetResolution(10);


    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(cone->GetOutputPort());
    // Previously I was doing this and it wasn't working:
    // mapper->SetInputData(cone->GetOutput());
    // Here the object being passed is a vtKPolyData*,
    // whereas in the above thing that works the object is vtkAlgorithmOutput*
    // Some explanation here about the recommended way to do it now:
    // https://vtk.org/Wiki/VTK/Tutorials/PipelineInput
    // Some explanation here about why one way works and the other doesn't, but in Chinese:
    // https://blog.csdn.net/yuyangyg/article/details/78165570

    auto coneActor = vtkSmartPointer<vtkActor>::New();
    coneActor->SetMapper(mapper);

    auto textActor = vtkSmartPointer<vtkTextActor>::New();
    textActor->SetInput("Hello World");

    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground(0.7,0.7,0.9);
    renderer->AddActor(textActor);
    renderer->AddActor(coneActor);

    auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    
    auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);

    // These two seem to be equivalent? Pick one I guess?
    renderWindow->SetInteractor(interactor);
    renderWindow->SetSize(1000,1000);
    // interactor->SetRenderWindow(renderWindow);

    // Things seem to work without this:
    // renderWindow->Render();
    // I wonder why

    auto iStyle = vtkInteractorStyleSwitch::SafeDownCast(interactor->GetInteractorStyle());
    if (iStyle)
        iStyle->SetCurrentStyleToTrackballActor();
    else
        throw std::runtime_error("Failed o downcast to a vtkInteractorStyleSwitch!");
    
    interactor->Start();

    return 0;
}
