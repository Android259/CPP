#include <cmath>

int main(){

    cout << std::endl << "Enter the number of steps : " << std::flush;
    std::cin >> n;
    cout << std::endl << "Enter the beginning of the interval : " << std::flush;
    std::cin >> a;
    cout << std::endl << "Enter the scale off step : " << std::flush;
    std::cin >> h;
    double exp
    double* les_uns = new double[n+1];
    double* les_xns = new double[n+1];
    double phi(double x, double u){
        return 2*x*u;

    }

    edo_explicite(a,n,h,1,les_uns,les_xns);
    error(exp_x_sqrt, edo_explicite);
    delete[] les_uns;
    delete[] les_xns;
}
void edo_explicite(double a, double n, double  h, double uO, 
                        double (phi*)(double, double), 
                        double* les_uns, double* les_xns){
    int i;
    les_uns[0]=u0;
    les_xns[0]=a
    for (i = 1; i < n+1; i++)
        les_xns[i] = a + i*h;
        les_uns[i] = les_uns[i-1] + h*phi(les_xns[i-1],les_uns[i-1]);
}
void error(double (analitical_solution*)(double), double (digital_solution*)(double), double* les_xns, double* les_uns){

}
double exp_x_sqrt(double x)
{
    return exp(x * x);
}