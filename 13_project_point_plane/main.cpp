#include <vtkNew.h>
#include <vtkPlane.h>
#include <iostream>

int main() {

    vtkNew<vtkPlane> plane;
    plane->SetOrigin(0,0,0);
    plane->SetNormal(0,0,1);

    double p[] = {23,54,9.2};
    double p_proj[3];

    plane->ProjectPoint(p,p_proj);
    std::cout << "Projected: " << p_proj[0] << " " << p_proj[1] << " "
              << p_proj[2] << std::endl;
              
    return EXIT_SUCCESS;
}