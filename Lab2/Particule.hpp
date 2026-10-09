class Particule {
        private:
        double x, y, z;
        double vx, v, vz;
        double masse;
        int id;
        int type;
        double fx, fy, fz;
        

        public:
        // constructeur
        Particule(double masse); 
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


        void setX(double x) { this->x = x; }
        void setY(double y) { this->y = y; }
        void setZ(double z) { this->z = z; }

        void setVx(double vx) { this->vx = vx; }
        void setVy(double vy) { this->vy = vy; }
        void setVz(double vz) { this->vz = vz; }

        void setFx(double fx) { this->fx = fx; }
        void setFy(double fy) { this->fy = fy; }
        void setFz(double fz) { this->fz = fz; }


        double gravitational_force_ij(const std::vector<Particule>& particules_vector, int i, int j, 
                                        double dx, double dy, double dz){ // calcul de force

                double r = std::sqrt(dx*dx + dy*dy + dz*dz);

                return particules_vector[i].getMasse() * particules_vector[j].getMasse()
                                                        / (r*r*r);
                
        }

        double gravitational_force_ij(const std::vector<Particule>& particules_vector, int i, int j){

                double dx = particules_vector[j].getX() - particules_vector[i].getX();
                double dy = particules_vector[j].getY() - particules_vector[i].getY();
                double dz = particules_vector[j].getZ() - particules_vector[i].getZ();

                return gravitational_force_ij(particules_vector, i, j, dx, dy, dz);
        }


        void renew_gravitational_force_dxdydz_i(std::vector<Particule>& particules_vector, int i){
                
                for (int j = i + 1; j < particules_vector.size(); ++j) {

                        double dx = particules_vector[j].getX() - particules_vector[i].getX();
                        double dy = particules_vector[j].getY() - particules_vector[i].getY();
                        double dz = particules_vector[j].getZ() - particules_vector[i].getZ();

                        double F_ij = gravitational_force_ij(particules_vector, i, j, dx, dy, dz);

                        // Fij
                        particules_vector[i].setFx(particules_vector[i].getFx() + F_ij * dx);
                        particules_vector[i].setFy(particules_vector[i].getFy() + F_ij * dy);
                        particules_vector[i].setFz(particules_vector[i].getFz() + F_ij * dz);

                        // Fji = -Fij
                        particules_vector[j].setFx(particules_vector[j].getFx() - F_ij * dx);
                        particules_vector[j].setFy(particules_vector[j].getFy() - F_ij * dy);
                        particules_vector[j].setFz(particules_vector[j].getFz() - F_ij * dz);
                }

        }
    };
