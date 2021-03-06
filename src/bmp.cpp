#include "bmp.h"
#include "lodepng.h"
#include <vector>


Bmp::Bmp(int _m, int _n)
{
    m = _m;
    n = _n;
    data = NULL;
    if(m!= 0 && n!= 0)
    {
        data = new Imgdata*[m];
        for(int i = 0; i < m; i++)
        {
            data[i] = new Imgdata[n];
            for(int j = 0; j < n; j++)
            {
                data[i][j].r = 0;
                data[i][j].g = 0;
                data[i][j].b = 0;
            }
        }
    }
}

Bmp::~Bmp()
{
    if(data)
    {
        for(int i = 0; i < m; i++)
            if(data[i]) delete [] data[i];
        if(data)    delete [] data;
        data = NULL;
    }
}


void Bmp::SetColor(int i, int j, const Color& c)
{
    data[i][j].r = byte(c.r * 255);
    data[i][j].g = byte(c.g * 255);
    data[i][j].b = byte(c.b * 255);
}

Color Bmp::GetColor(int i, int j)
{
    double r,g,b;
    r = double(data[i][j].r) / 255.0;
    g = double(data[i][j].g) / 255.0;
    b = double(data[i][j].b) / 255.0;
    return Color(r,g,b);
}


void Bmp::Input(str file)
{
    file = "../input/" + file;
    std::vector<unsigned char> image;
    unsigned w, h;
    unsigned error = lodepng::decode(image, w, h, file, LCT_RGB);
    m = h;n = w;
    data = new Imgdata*[m];
    for(int i = 0; i < m; i++)
    {
        data[i] = new Imgdata[n];
        for(int j = 0; j < n; j++)
        {
            data[i][j].r = image[(i * n + j) * 3 + 0];
            data[i][j].g = image[(i * n + j) * 3 + 1];
            data[i][j].b = image[(i * n + j) * 3 + 2];
        }
    }
}

void Bmp::Output(str file)
{
    std::cout << "Start Output" << std::endl;
    std::vector<unsigned char> image(m * n * 3);
    for(int i = 0; i < m; i++)
        for(int j = 0; j < n; j++)
        {
            image[(i * n + j) * 3 + 0] = data[i][j].r;
            image[(i * n + j) * 3 + 1] = data[i][j].g;
            image[(i * n + j) * 3 + 2] = data[i][j].b;
        }
    unsigned error = lodepng::encode(file, image, n, m, LCT_RGB);
    std::cout << "Output Over" << std::endl;
}
