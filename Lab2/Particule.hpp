class Particule {
    private:
    double x; y; z;
    double vx; vy; vz;
    double masse;
    int id;
    int type;
    double fx, fy, fz;
    

    public:
    Particule(double masse); // constructeur
    Particule(double x, double y, double z,
            double vx, double vy, double vz,
            double masse, int id, int type,
            double fx, double fy, double fz)
            : x(x), y(y), z(z),
            vx(vx), vy(vy), 
            vz(vz),masse(masse),
            {}

    double getMasse() const { return masse; }
    void setMasse(double m) { masse = m; }

    int getId() const { return id; }
    void setId(int i) { id = i; }

    int getType() const { return type; }
    void setType(int t) { type = t; }

    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }

    double getVx() const { return vx; }
    double getVy() const { return vy; }
    double getVz() const { return vz; }

    double getFx() const { return fx; }
    double getFy() const { return fy; }
    double getFz() const { return fz; }
    };
