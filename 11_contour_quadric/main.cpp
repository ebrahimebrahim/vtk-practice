#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkOutlineFilter.h>
#include <vtkContourFilter.h>
#include <vtkQuadric.h>
#include <vtkSampleFunction.h>
#include <vtkImageData.h>
#include <vtkPointData.h>

#include <iostream>





int main() {

    vtkNew<vtkNamedColors> colors;
   
    vtkNew<vtkRenderer> r;
    r->SetBackground(colors->GetColor3d("Silver").GetData());

    vtkNew<vtkRenderWindow> w;
    w->AddRenderer(r.GetPointer());
    w->SetSize(600,600);
    w->SetWindowName("Contour Quadric Example");

    vtkNew<vtkRenderWindowInteractor> i;
    i->SetRenderWindow(w.GetPointer());
    vtkNew<vtkInteractorStyleTrackballCamera> style;
    i->SetInteractorStyle(style.GetPointer());


    // The vtkQuadric is a vtkImplicitFunction that represents the quadric function as a polynomial function
    // This is not part of the visualization pipeline, but just a parameter that will going into vtkSampleFunction
    vtkNew<vtkQuadric> quadric;
    quadric->SetCoefficients(.5, 1, .2, 0, .1, 0, 0, .2, 0, 0);

    // The vtkSampleFunction is the first part of our visualization. It is a source, 
    // a specification of vtkImageAlgorithm
    // It will produce as its output a vtkImageData, which is a specialized vtkDataSet
    // The data consists of samples of the quadric function values.
    // We don't actually interact directly with the vtkDataSet that it produces,
    // instead we pipe this vtkImageAlgorithm into the next process in the visualization pipeline,
    // which will be a vtkContourFilter.
    vtkNew<vtkSampleFunction> sample;
    sample->SetSampleDimensions(50,50,50);
    // Wondering what points in space that 50x50x50 grid is covering? See use of "GetModelBounds" below to see default.
    sample->SetImplicitFunction(quadric.GetPointer());

    // The vtkContourFilter is a specialized vtkPolyDataAlgorithm which will take
    // the grid of sampled values and produce a vtkPolyData as output.
    // This probably contains a pretty cool algorithm!
    vtkNew<vtkContourFilter> contour;
    contour->SetInputConnection(sample->GetOutputPort());
    contour->GenerateValues(5,0,1.2);

    vtkNew<vtkPolyDataMapper> contourMapper;
    contourMapper->SetInputConnection(contour->GetOutputPort());
    contourMapper->SetScalarRange(0,1.2);

    vtkNew<vtkActor> contourActor;
    contourActor->SetMapper(contourMapper.GetPointer());

    r->AddActor(contourActor.GetPointer());


    i->Start();

    // What if we do want to interact directly with the vtkDataSet that the vtkSampleFunction produces?
    // I guess we could do it like this:
    vtkImageData * sampleData = sample->GetOutput();
    // sampleData is a vtkDataSet which contains a 3D grid of points in the form of a vtkPointData:
    vtkPointData * samplePoints = sampleData->GetPointData();
    // a vtkPointData is a specialization of vtkDataSetAttributes
    // This one contains "scalars" and for some reason that I can't make out it also contains "normals".
    // Perhaps "normals" refers to the vectors pointing out the locations in space of the samples?
    // It would make sense that we have to keep that data in the structure, for the next step to be able to draw contours in space.
    // But why is it in the "normals" and not the "vectors" part of the vtkDataSetAttributes?
    // Let's look at scalars: 
    vtkDataArray * sampleScalarArray = samplePoints->GetScalars();
    std::cout << "Here's the first value, at index 0,0,0 of the array: " << sampleScalarArray->GetTuple1(0) << std::endl;
    // We get 1.6. If you run the mathematica code I included in this example, you see that this is indeed
    // the value of the function at (-1,-1,-1), which I imagine is the point sampled at index 0.
    // Let's look at the "normals" array and see if it has (-1,-1,-1) at index 0.
    vtkDataArray * sampleNormals = samplePoints->GetNormals();
    double* normal = sampleNormals->GetTuple3(0);
    std::cout << "Here's the first \"normal\", at index 0,0,0 of the array: (" <<
        normal[0] << ", " <<
        normal[1] << ", " <<
        normal[2] << ")" << std::endl;
    // Oh hmm it's not (-1,-1,-1)
    // OOOOHHH it really is a *normal* vector to the isosurface at that point! The gradient of the quadric function provides it,
    // so it makes a lot of sense that it would be included in the data structure.
    // Okay then where is the (-1,-1,-1) stored? How does sampleData know what locations in space the samples are for?
    // OOH the location is not stored for each point, it's just bounds, which are stored in sampleData as xmin, xmax, ymin, ymax, zmin, zmax
    // Makes  a lot of sense. The next step in the pipeline can use the bounds to position polygons in space.

    // I'm curious what region of space was chosen by default, because we didn't set it in this example.
    double bnds[6];
    sample->GetModelBounds(bnds);
    std::cout << 
        "x range: " << bnds[0] << " to " << bnds[1] << std::endl <<
        "y range: " << bnds[2] << " to " << bnds[3] << std::endl <<
        "z range: " << bnds[4] << " to " << bnds[5] << std::endl ;
    // It prints -1 to 1 for all of them.
        





    return 0;
}