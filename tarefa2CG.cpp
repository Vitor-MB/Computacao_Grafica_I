#include <bits/stdc++.h>

using namespace std;

typedef struct RGBn { double r, g, b;} RGBn;

//Cores
typedef struct RGB { 
    unsigned int r, g, b;

    RGBn operator/(double k) const{
        return RGBn{(double)r/k, (double)g/k, (double)b/k};
    }
} RGB;

//Montando o canvas
typedef struct Canvas{
    int w, h;
    vector<RGB> janela;

    Canvas(int w, int h){
        this->w = w;
        this->h = h;
        this->janela = vector<RGB> (w*h, {0, 0, 0});
    }

    void GeraImg(const string& filename){
        ofstream out(filename);
        out << "P3\n" << w << " " << h << "\n255\n";
        for(int i=0; i<w*h; i++){
            out << janela[i].r << " " << janela[i].g << " " << janela[i].b << "\n";
        }
        out.close();
    }
} Canvas;
//Vetr
typedef struct Vt{
    double x, y, z;

    Vt(double x=0, double y=0, double z=0){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    //Subtracao de Vetores
    Vt operator-(const Vt &v) const{
        return Vt(x - v.x, y-v.y, z-v.z);
    }

    Vt operator+(const Vt &v) const{
        return Vt(x + v.x, y+v.y, z+v.z);
    }

    Vt operator*(double k) const{
        return Vt(x*k, y*k, z*k);
    }

    //Produto Escalar
    double ProdEsc(Vt &v){
        return (x*v.x)+(y*v.y)+(z*v.z);
    }

    double norma(){
        return sqrt(this->ProdEsc(*this));
    }

    Vt normalizado(){
        double n = sqrt(x*x + y*y + z*z);
        return Vt(x/n, y/n, z/n);
    }

} Vt;

typedef struct luz{
    RGB I_F;
    RGBn CorNormalizada;
    Vt P_F;

    luz(double r, double g, double b, double x, double y, double z){
        
        unsigned int red = r*255;
        unsigned int green = g*255;
        unsigned int blue = b*255;

        this->CorNormalizada = {r, g, b};
        this->I_F = {red, green, blue};
        this->P_F = {x, y, z};
    }

    
} luz;

bool IntersectaEsfera(Vt &O, Vt &D, Vt &C, double r, double &t){
    Vt OrCe = O - C;
    double a = D.ProdEsc(D);
    double b = 2 * OrCe.ProdEsc(D);
    double c = OrCe.ProdEsc(OrCe) - (r*r);
    double Dlt = (b*b) - (4*a*c);

    if(Dlt < 0) return false;
    
    double t1 = (-b + sqrt(Dlt)) / (2*a);
    double t2 = (-b - sqrt(Dlt)) / (2*a);

    if(t1 > 0 && t2 > 0) {
        t = min(t1, t2);
        return true;
    }
    else if(t1 > 0){
        t = t1;
        return true;
    }
    else if(t2 > 0){
        t = t2;
        return true;
    }
    else return false;
}



int main(){

    float wJanela = 4;
    float hJanela = 4;
    float dJanela = 2;

    int rEsfera = 1;
    Vt Centro = Vt(0,0, -(dJanela + rEsfera));

    Vt Eye = Vt(0, 0, 0);

    RGB esfColor = {255,0,0};
    RGB bgColor = {100,100,100};

    int nCol = 800;
    int nLin = 800;

    //Propriedades de reflexão da esfera
    RGBn Kdif = esfColor / 255.0;
    RGBn Kesp = {1.0, 1.0, 1.0};
    int m = 8;

    double Dx = wJanela/nCol;
    double Dy = hJanela/nLin;

    luz Luz = luz(0.7, 0.7, 0.7, 0, 5, 0);

    Canvas canvas(nCol, nLin);

    for(int l=0; l<nLin; l++){
        for(int c=0; c<nCol; c++){
            double x = -wJanela/2 + Dx/2 + c*Dx;
            double y = hJanela/2 - Dy/2 - l*Dy;

            Vt P = Vt(x, y , -dJanela);
            Vt D = (P-Eye).normalizado();
           
            double t;
            if(IntersectaEsfera(Eye, D, Centro, rEsfera, t)){
                Vt Pp = Eye + D*t;

                Vt Normal = (P - Centro).normalizado();

                
                Vt Ldir = (Luz.P_F - Pp).normalizado();
                Vt Vdir = (Eye - Pp).normalizado();
                Vt R = (Normal * (2*Normal.ProdEsc(Ldir)) - Ldir).normalizado();

                double ldotn = max(0.0, Normal.ProdEsc(Ldir));
                double vdotr = max(0.0, Vdir.ProdEsc(R));

                RGBn I_d = {Luz.CorNormalizada.r * Kdif.r * ldotn,
                              Luz.CorNormalizada.g * Kdif.g * ldotn,
                              Luz.CorNormalizada.b * Kdif.b * ldotn};

                double fatorEsp = pow(vdotr, m);
                RGBn I_e = {Luz.CorNormalizada.r * Kesp.r * fatorEsp,
                             Luz.CorNormalizada.g * Kesp.g * fatorEsp,
                             Luz.CorNormalizada.b * Kesp.b * fatorEsp};

                int r = min(255, int((I_d.r + I_e.r)*255));
                int g = min(255, int((I_d.g + I_e.g)*255));
                int b = min(255, int((I_d.b + I_e.b)*255));

                canvas.janela[l*nCol +c] = {(unsigned int)r, (unsigned int)g, (unsigned int)b};
            }
            else{
                canvas.janela[l*nCol +c] = bgColor;
            }
        }
    }

    canvas.GeraImg("tarefa2.ppm");

    return 0;
}