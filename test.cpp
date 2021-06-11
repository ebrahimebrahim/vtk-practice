#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkInteractionStyle);

#include <vtkSmartPointer.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkTextActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkActor.h>

int main()
{
    auto cyl = vtkSmartPointer<vtkCylinderSource>::New();

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(cyl->GetOutputPort());
    // SetInputData? GetOutput?

    auto cylActor = vtkSmartPointer<vtkActor>::New();
    cylActor->SetMapper(mapper);

    auto textActor = vtkSmartPointer<vtkTextActor>::New();
    textActor->SetInput("Hello World");

    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(textActor);
    renderer->AddActor(cylActor);

    auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    
    auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);

    // These two seem to be equivalent? Pick one I guess?
    renderWindow->SetInteractor(interactor);
    // interactor->SetRenderWindow(renderWindow);

    // Things seem to work without this:
    // renderWindow->Render();
    // I wonder why
    
    interactor->Start();

    return 0;
}
