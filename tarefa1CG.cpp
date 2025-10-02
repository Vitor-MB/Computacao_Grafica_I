#include <bits/stdc++.h>

using namespace std;

//Cores
typedef struct RGB { unsigned int r, g, b;} RGB;

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
    Vt operator-(Vt &v){
        return Vt(x - v.x, y-v.y, z-v.z);
    }

    //Produto Escalar
    double ProdEsc(Vt &v){
        return (x*v.x)+(y*v.y)+(z*v.z);
    }

} Vt;

typedef struct luz{
    RGB I_F;
    Vt P_F;

    luz(double r, double g, double b, double x, double y, double z){
        unsigned int red = r*255;
        unsigned int green = g*255;
        unsigned int blue = b*255;

        this->I_F = {red, green, blue};
        this->P_F = {x, y, z};
    }

    
} luz;

bool IntersectaEsfera(Vt &O, Vt &D, Vt &C, double r){
    Vt OrCe = O - C;
    double a = D.ProdEsc(D);
    double b = 2 * OrCe.ProdEsc(D);
    double c = OrCe.ProdEsc(OrCe) - (r*r);
    double Dlt = (b*b) - (4*a*c);
    return Dlt>=0;
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
    RGB Kdif = esfColor;
    RGB Kesp = {255, 255, 255};
    int m = 10;

    double Dx = wJanela/nCol;
    double Dy = hJanela/nLin;

    Canvas canvas(nCol, nLin);

    for(int l=0; l<nLin; l++){
        for(int c=0; c<nCol; c++){
            double x = -wJanela/2 + Dx/2 + c*Dx;
            double y = hJanela/2 - Dy/2 - l*Dy;
            Vt P = Vt(x, y , -dJanela);
            Vt D = P-Eye;

            double norma = sqrt(D.ProdEsc(D));
            D = Vt(D.x/norma, D.y/norma, D.z/norma);
           

            if(IntersectaEsfera(Eye, D, Centro, rEsfera)){
                canvas.janela[l*nCol +c] = esfColor;
            }
            else{
                canvas.janela[l*nCol +c] = bgColor;
            }
        }
    }

    canvas.GeraImg("tarefa1.ppm");

    return 0;
}
