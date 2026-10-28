/// Lab 1 : u'(x) = 2*x*u(x) resolu par Euler explicite, compare a la solution
/// exacte u(x) = exp(x^2) (cas a = 0, u0 = 1).

#include <cmath>
#include <cstdio>
#include <vector>

/// Membre de droite de l'EDO : u' = phi(x, u).
double phi(double x, double u) {
    return 2.0*x*u;
}

/// Solution analytique pour a = 0 et u0 = 1.
double solutionExacte(double x) {
    return std::exp(x*x);
}

/// Euler explicite : u_{n+1} = u_n + h*phi(x_n, u_n).
/// Remplit les n+1 abscisses et valeurs calculees.
void eulerExplicite(double a, int n, double h, double u0,
                    double (*f)(double, double),
                    std::vector<double>& xs, std::vector<double>& us) {
    xs.assign(static_cast<std::size_t>(n) + 1, 0.0);
    us.assign(static_cast<std::size_t>(n) + 1, 0.0);
    xs[0] = a;
    us[0] = u0;
    for (int i = 1; i <= n; ++i) {
        xs[i] = a + i*h;
        us[i] = us[i - 1] + h*f(xs[i - 1], us[i - 1]);
    }
}

/// Erreur maximale (norme infinie) entre solution numerique et solution exacte.
double erreurMax(const std::vector<double>& xs, const std::vector<double>& us,
                 double (*exacte)(double)) {
    double maximum = 0.0;
    for (std::size_t i = 0; i < xs.size(); ++i) {
        double ecart = std::fabs(us[i] - exacte(xs[i]));
        if (ecart > maximum) {
            maximum = ecart;
        }
    }
    return maximum;
}

int main() {
    const double a = 0.0;   // debut de l'intervalle
    const double b = 1.0;   // fin de l'intervalle
    const double u0 = 1.0;  // condition initiale u(a)

    std::vector<double> xs, us;

    const int n = 10;
    const double h = (b - a) / n;
    eulerExplicite(a, n, h, u0, phi, xs, us);

    std::printf("Euler explicite sur [%g, %g], n = %d, h = %g\n", a, b, n, h);
    std::printf("     x        u_num       u_exact      erreur\n");
    for (int i = 0; i <= n; ++i) {
        double exacte = solutionExacte(xs[i]);
        std::printf("%6.3f  %11.6f  %11.6f  %10.3e\n",
                    xs[i], us[i], exacte, std::fabs(us[i] - exacte));
    }

    // L'erreur d'Euler explicite est en O(h)
    std::printf("\nConvergence (erreur max sur [%g, %g]) :\n", a, b);
    std::printf("     n          h      erreur_max    ratio\n");
    double erreurPrecedente = 0.0;
    for (int k = 1; k <= 8; ++k) {
        int nk = 10*(1 << k);
        double hk = (b - a) / nk;
        eulerExplicite(a, nk, hk, u0, phi, xs, us);
        double err = erreurMax(xs, us, solutionExacte);
        if (k == 1) {
            std::printf("%6d  %9.3e  %10.3e        -\n", nk, hk, err);
        } else {
            std::printf("%6d  %9.3e  %10.3e  %7.3f\n", nk, hk, err, erreurPrecedente / err);
        }
        erreurPrecedente = err;
    }
    std::printf("\nLe ratio tend vers 2 : l'erreur est bien en O(h), l'ordre 1\n"
                "attendu pour Euler explicite est donc verifie.\n");

    return 0;
}
