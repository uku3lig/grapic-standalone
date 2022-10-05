/****************************************************************************
Copyright (C) 2010-2020 Alexandre Meyer

This file is part of Grapic.

Grapic is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Grapic is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Grapic.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/


#include <Grapic.h>


namespace grapic
{

using namespace std;


Grapic* Grapic::currentGrapic = nullptr;


Grapic::Grapic() :
    m_width(-1),
    m_height(-1),
    m_window(nullptr),
    m_renderer(nullptr),
    m_font(nullptr),
    m_fontFile(""),
    m_fontSize(-1),
    m_quit(false),
    m_anim(false),
    m_keyRepeatMode(false),
    imagesSavedCount(0)
{
    m_currentColor.r = 0;
    m_currentColor.g = 0;
    m_currentColor.b = 0;
    m_currentColor.a = 255;
    m_backgroundColor.r = 255;
    m_backgroundColor.g = 255;
    m_backgroundColor.b = 255;
    m_backgroundColor.a = 255;

    if (Grapic::currentGrapic)
        delete Grapic::currentGrapic;
    Grapic::currentGrapic = this;
}


void Grapic::init(const char* name, int w, int h, int posx, int posy, SDL_WindowFlags flag, std::string render_driver)
{
    SDL_version compiled;
    SDL_version linked;

    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);
    printf("SDL version (compiled) %d.%d.%d ...\n", compiled.major, compiled.minor, compiled.patch);
    printf("SDL version (linked) %d.%d.%d.\n", linked.major, linked.minor, linked.patch);


    // Initialisation de la SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << std::endl;
        SDL_Quit();
        assert(0);
        exit(1);
    }

    if (render_driver!="")
        SDL_SetHint(SDL_HINT_RENDER_DRIVER, render_driver.c_str()); //"opengl");

    if (TTF_Init() != 0)
    {
        std::cout << "Erreur lors de l'initialisation de la SDL_ttf : " << SDL_GetError() << std::endl;
        SDL_Quit();
        assert(0);
        exit(1);
    }

    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        SDL_Quit();
        assert(0);
        exit(1);
    }

    //system("cd");
    //setFont( 20, "data/ttf/Raleway-Regular.ttf");
    setFont( 20, "data/ttf/Roboto-Regular.ttf");

    // Creation de la fenetre
    if (m_window) SDL_DestroyWindow(m_window);
    if (posx<0) posx = SDL_WINDOWPOS_CENTERED;
    if (posy<0) posy = SDL_WINDOWPOS_CENTERED;
    m_window = SDL_CreateWindow(name, posx, posy, w, h, SDL_WINDOW_SHOWN | flag ); //| SDL_WINDOW_RESIZABLE);
    if (m_window == nullptr)
    {
        std::cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << std::endl;
        SDL_Quit();
        assert(0);
        exit(1);
    }

    SDL_SetWindowTitle(m_window, name);

    m_renderer = SDL_CreateRenderer( m_window, -1, SDL_RENDERER_ACCELERATED);
    SDL_GetRendererInfo( m_renderer, &m_renderInfo);
    cout<<"renderer:"<<endl;
    cout<<"  name: "<<m_renderInfo.name<<endl;
    cout<<"  flag: "<<m_renderInfo.flags<<" ";
    if (m_renderInfo.flags & SDL_RENDERER_SOFTWARE) { cout << "  SDL_RENDERER_SOFTWARE "; }
    if (m_renderInfo.flags & SDL_RENDERER_ACCELERATED) { cout << "  SDL_RENDERER_ACCELERATED "; }
    if (m_renderInfo.flags & SDL_RENDERER_PRESENTVSYNC) { cout << "  SDL_RENDERER_PRESENTVSYNC "; }
    if (m_renderInfo.flags & SDL_RENDERER_TARGETTEXTURE) { cout << "  SDL_RENDERER_TARGETTEXTURE "; }
    cout<<endl;

    cout << "   Texture Formats: nb="<<m_renderInfo.num_texture_formats<<"  ";
    for(size_t i = 0; i < m_renderInfo.num_texture_formats; ++i)
    {
      cout << "  " << SDL_GetPixelFormatName(m_renderInfo.texture_formats[i]) << " ";
    }
    assert( m_renderInfo.num_texture_formats>=1 );
    m_textureFormat = m_renderInfo.texture_formats[0];
    cout<<endl;
    cout<<"   texture format default: "<<SDL_GetPixelFormatName(m_textureFormat)<<"("<<m_textureFormat<<")"<<endl;

    cout << "   Max Texture Width: " << m_renderInfo.max_texture_width << std::endl;
    cout << "   Max Texture Height: " << m_renderInfo.max_texture_height << std::endl;
    cout<<"----------"<<endl;


    m_width = w;
    m_height = h;
    m_quit = false;
    m_anim = false;

    SDL_SetRenderDrawBlendMode( m_renderer, SDL_BLENDMODE_BLEND);

    initKeyArray();
    help();
}


const SDL_Surface* Image::surface() const
{
    return m_surface;
}

    bool Image::isInit() const { return m_surface && m_texture; }


Menu::Menu() : m_select(0), m_visible(true) {}

void Menu::change(int i, const std::string& str)
{
    if ((i>=0) && (i<m_txt.size()))
        m_txt[i] = str;
    else
        std::cerr<<"menu_change(...): i is not in the range of the menu"<<std::endl;
}

    Plot::Plot() : m_nb_plot_max(-1) {}

    void Plot::clear()
    {
        for(int i=0; i<m_dat.size(); ++i)
            m_dat[i].clear();
        m_dat.clear();
    }

    void Plot::setSize(const int n)
    {
        clear();
        m_nb_plot_max = n;
    }


int Menu::select() const
{
    return m_select;
}
void Menu::setSelect(int s)
{
    assert(s>=0);
    assert(s<m_txt.size());
    m_select=s;
}
int Menu::caseToPixel(int c, int ymin, int ymax) const
{
    return ymin + (m_txt.size()-c) * ( (ymax-ymin+1)/m_txt.size() );
}

void Menu::add(const std::string& str)
{
    m_txt.push_back(str);
}


bool saveScreenshotPNG(std::string filepath, SDL_Window* SDLWindow, SDL_Renderer* SDLRenderer)
{
    SDL_Surface* saveSurface = nullptr;
    SDL_Surface* infoSurface = nullptr;
    infoSurface = SDL_GetWindowSurface(SDLWindow);
    if (infoSurface == nullptr)
    {
        std::cerr << "Failed to create info surface from window in saveScreenshotBMP(string), SDL_GetError() - " << SDL_GetError() << "\n";
    }
    else
    {
        unsigned char * pixels = new (std::nothrow) unsigned char[infoSurface->w * infoSurface->h * infoSurface->format->BytesPerPixel];
        if (pixels == 0)
        {
            std::cerr << "Unable to allocate memory for screenshot pixel data buffer!\n";
            return false;
        }
        else
        {
            if (SDL_RenderReadPixels(SDLRenderer, &infoSurface->clip_rect, infoSurface->format->format, pixels, infoSurface->w * infoSurface->format->BytesPerPixel) != 0)
            {
                std::cerr << "Failed to read pixel data from SDL_Renderer object. SDL_GetError() - " << SDL_GetError() << "\n";
                pixels = nullptr;
                return false;
            }
            else
            {
                saveSurface = SDL_CreateRGBSurfaceFrom(pixels, infoSurface->w, infoSurface->h, infoSurface->format->BitsPerPixel, infoSurface->w * infoSurface->format->BytesPerPixel, infoSurface->format->Rmask, infoSurface->format->Gmask, infoSurface->format->Bmask, infoSurface->format->Amask);
                if (saveSurface == NULL)
                {
                    std::cerr << "Couldn't create SDL_Surface from renderer pixel data. SDL_GetError() - " << SDL_GetError() << "\n";
                    return false;
                }
                //SDL_SaveBMP(saveSurface, filepath.c_str());
                IMG_SavePNG(saveSurface, filepath.c_str());
                SDL_FreeSurface(saveSurface);
                saveSurface = nullptr;
            }
            delete[] pixels;
        }
        SDL_FreeSurface(infoSurface);
        infoSurface = nullptr;
    }
    return true;
}

bool Grapic::manageEvent()
{
    SDL_Event event;
    SDL_PumpEvents();
    while (SDL_PollEvent(&event))
    {
        manageOneEvent(event);
    }
    return m_quit;
}

bool Grapic::manageOneEvent(SDL_Event event)
{
    switch (event.type)
    {
    case SDL_QUIT:
        m_quit = true;
        break;
    case SDL_KEYDOWN:
        if (((size_t)event.key.keysym.scancode < m_keyStates.size()) && ((m_keyRepeatMode) || (event.key.repeat == 0)))
        {
            //cout<<event.key.keysym.scancode<<endl;
            m_keyStates[event.key.keysym.scancode]++; // = 1;
            //last_key= event.key;    // conserver le dernier evenement
        }

        if ((event.key.keysym.sym == SDLK_ESCAPE) || (event.key.keysym.sym == SDLK_q))
            m_quit = true;
        else if (event.key.keysym.sym == SDLK_F12)
        {
            char filename[128];
            sprintf(filename, "grapic_%d.png", imagesSavedCount);
            imagesSavedCount++;
            saveScreenshotPNG(filename, m_window, m_renderer);
            printf("Save %s\n", filename);
        }
        else if (event.key.keysym.sym == SDLK_h)
        {
            help();
        }
        break;
    case SDL_KEYUP:
        // modifier l'etat du clavier
        if ((size_t)event.key.keysym.scancode < m_keyStates.size())
        {
            m_keyStates[event.key.keysym.scancode] = 0;
            //last_key= event.key;    // conserver le dernier evenement
        }
        break;
    default:
        break;
        }
    return m_quit;
}


void Grapic::clearEvent()
{
    SDL_Event events;
    SDL_PumpEvents();
    while (SDL_PollEvent(&events))
    {
    }
    initKeyArray();
}


Grapic& Grapic::singleton(bool secure)
{
    if (secure)
    {
        if (!currentGrapic->isInit())
        {
            std::cout<<"You have to call winInit before any call to Grapic functions !"<<std::endl;
            exit(1);
        }
    }
    return *currentGrapic;
}


void Grapic::help() const
{
    printf("Help:\n");
    printf("   q,ESC: quit\n");
    printf("   F12: save the screen\n");
    printf("   F1: hide/show the menu (if you use one in your program)\n");
    printf("   h: help\n");
}


int Grapic::keyHasBeenPressed(unsigned int key)
{
    SDL_Scancode code= SDL_GetScancodeFromKey(key);
    assert((size_t) code < m_keyStates.size());

    int res = (int)  m_keyStates[code];
    if (!m_keyRepeatMode)
    {
        m_keyStates[code]= 0;
    }
    return res;
}


void Grapic::initKeyArray()
{
    int keys;
    const unsigned char *state= SDL_GetKeyboardState(&keys);
    m_keyStates.assign(state, state + keys);
}


void Grapic::clear()
{
    // Clear the entire screen to our selected color.
    SDL_SetRenderDrawColor(m_renderer, m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, m_backgroundColor.a);
    SDL_RenderClear(m_renderer);
    SDL_SetRenderDrawColor(m_renderer, m_currentColor.r, m_currentColor.g, m_currentColor.b, m_currentColor.a);
}


bool Grapic::display()
{
    manageEvent();
    SDL_RenderPresent(m_renderer);
    return m_quit;
}


void Grapic::quit()
{
    TTF_CloseFont(m_font);
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
    m_font = nullptr;
    m_window = nullptr;
    m_renderer = nullptr;
    SDL_Quit();
}


void Grapic::setKeyRepeatMode(bool kr)
{
    m_keyRepeatMode = kr;
}

void Grapic::color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    m_currentColor.r = r;
    m_currentColor.g = g;
    m_currentColor.b = b;
    m_currentColor.a = a;
    SDL_SetRenderDrawColor(m_renderer, m_currentColor.r, m_currentColor.g, m_currentColor.b, m_currentColor.a);
}

void Grapic::colorf(float r, float  g, float  b, float  a)
{
    m_currentColor.r = (unsigned char)( std::clamp(255*r,0.f,255.f) );
    m_currentColor.g = (unsigned char)( std::clamp(255*g,0.f,255.f) );
    m_currentColor.b = (unsigned char)( std::clamp(255*b,0.f,255.f) );
    m_currentColor.a = (unsigned char)( std::clamp(255*a,0.f,255.f) );
    SDL_SetRenderDrawColor(m_renderer, m_currentColor.r, m_currentColor.g, m_currentColor.b, m_currentColor.a);
}

SDL_Color& Grapic::getColor()
{
    return m_currentColor;
}

SDL_Color& Grapic::getBackgroundColor()
{
    return m_backgroundColor;
}

void Grapic::backgroundColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    m_backgroundColor.r = r;
    m_backgroundColor.g = g;
    m_backgroundColor.b = b;
    m_backgroundColor.a = a;
}

void Grapic::backgroundColorf(float r, float g, float b, float a)
{
    m_backgroundColor.r = (unsigned char)( std::clamp(255*r,0.f,255.f) );
    m_backgroundColor.g = (unsigned char)( std::clamp(255*g,0.f,255.f) );
    m_backgroundColor.b = (unsigned char)( std::clamp(255*b,0.f,255.f) );
    m_backgroundColor.a = (unsigned char)( std::clamp(255*a,0.f,255.f) );
}

int Grapic::inverseY(int y)
{
    SDL_GetRendererOutputSize(m_renderer, &m_width, &m_height);
    return m_height - 1 - y;
}


void Grapic::setFont(int s, const char* ttf)
{
    if ((m_font) && (m_fontSize==s) && (ttf && m_fontFile==std::string(ttf))) return;
    if (m_font) TTF_CloseFont(m_font);
    m_fontSize = s;
    if (ttf) m_fontFile = ttf;
    m_font = TTF_OpenFont( m_fontFile.c_str(), m_fontSize);
    if (!m_font) m_font = TTF_OpenFont( ("../"+m_fontFile).c_str(), m_fontSize);
    if (!m_font) m_font = TTF_OpenFont( ("../../"+m_fontFile).c_str(), m_fontSize);
    if (!m_font) m_font = TTF_OpenFont( ("../../../"+m_fontFile).c_str(), m_fontSize);
    if (!m_font) m_font = TTF_OpenFont( ("../../../../"+m_fontFile).c_str(), m_fontSize);
    if (!m_font)
    {
        std::cout << "Erreur lors de l'initialisation de la police : " << SDL_GetError() << std::endl;
        SDL_Quit();
        assert(0);
        exit(1);
    }
}



//==================================================================================================
//==================================================================================================
//==================================================================================================
//============================= GRAPIC FUNCTIONS ===================================================
//==================================================================================================
//==================================================================================================
//==================================================================================================


inline SDL_Renderer* renderer()
{
    return Grapic::singleton().renderer();
}

void winSetPosition(int w, int h, int px, int py, bool fullscreen)
{
    if (fullscreen) SDL_SetWindowFullscreen( Grapic::singleton().window(), SDL_WINDOW_FULLSCREEN );
    else SDL_SetWindowFullscreen( Grapic::singleton().window(), 0);

    int x,y;
    SDL_GetWindowSize( Grapic::singleton().window(), &x, &y);
    if (w<0) w=x;
    if (h<0) h=y;
    SDL_SetWindowSize( Grapic::singleton().window(), w, h);


    SDL_GetWindowPosition( Grapic::singleton().window(), &x, &y);
    if (px<0) px=x;
    if (py<0) py=y;
    SDL_SetWindowPosition( Grapic::singleton().window(), px,py);
}

void pressSpace(bool isPrint)
{
    winClearEvent();
    winDisplay();
    if (isPrint) print(10,10,"Press space");
    winDisplay();
    if (isPrint) printf("Press space\n");

    while(!winHasFinished() && !isKeyPressed(' '))
    {
        Grapic::singleton().manageEvent();
        delay(50);
    }
    Grapic::singleton().manageEvent();
    delay(50);
}

void rectangle(int xmin, int ymin, int xmax, int ymax)
{
    Grapic& g = Grapic::singleton();
    SDL_Rect r;
    r.x = xmin;
    r.y = g.inverseY(ymax);
    r.w = xmax - xmin + 1;
    r.h = ymax - ymin + 1;
    SDL_RenderDrawRect(renderer(), &r);
}

void rectangleFill(int xmin, int ymin, int xmax, int ymax)
{
    Grapic& g = Grapic::singleton();
    SDL_Rect r;
    r.x = xmin;
    r.y = g.inverseY(ymax);
    r.w = xmax - xmin + 1;
    r.h = ymax - ymin + 1;
    SDL_RenderFillRect(renderer(), &r);
}

int filledEllipseRGBA(SDL_Renderer* m_renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int result;
    int ix, iy;
    int h, i, j, k;
    int oh, oi, oj, ok;
    int xmh, xph;
    int xmi, xpi;
    int xmj, xpj;
    int xmk, xpk;

    /*
    * Sanity check radii
    */
    if ((rx < 0) || (ry < 0))
    {
        return (-1);
    }

    /*
    * Special case for rx=0 - draw a vline
    */
    if (rx == 0)
    {
        return SDL_RenderDrawLine(m_renderer, x, y-ry, x, y+ry);
        //return (vlineRGBA(renderer, x, y - ry, y + ry, r, g, b, a));
    }
    /*
    * Special case for ry=0 - draw a hline
    */
    if (ry == 0)
    {
        return SDL_RenderDrawLine(m_renderer, x-rx, y, x+rx, y);
        //		return (hlineRGBA(renderer, x - rx, x + rx, y, r, g, b, a));
    }

    /*
    * Set color
    */
    result = 0;
    result |= SDL_SetRenderDrawBlendMode(m_renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
    result |= SDL_SetRenderDrawColor(m_renderer, r, g, b, a);

    /*
    * Init vars
    */
    oh = oi = oj = ok = 0xFFFF;

    /*
    * Draw
    */
    if (rx > ry)
    {
        ix = 0;
        iy = rx * 64;

        do
        {
            h = (ix + 32) >> 6;
            i = (iy + 32) >> 6;
            j = (h * ry) / rx;
            k = (i * ry) / rx;

            if ((ok != k) && (oj != k))
            {
                xph = x + h;
                xmh = x - h;
                if (k > 0)
                {
                    result |= SDL_RenderDrawLine(m_renderer, xmh, y+k, xph, y+k); //hline(renderer, xmh, xph, y + k);
                    result |= SDL_RenderDrawLine(m_renderer, xmh, y-k, xph, y-k); //hline(renderer, xmh, xph, y - k);
                }
                else
                {
                    result |= SDL_RenderDrawLine(m_renderer, xmh, y, xph, y); // hline(renderer, xmh, xph, y);
                }
                ok = k;
            }
            if ((oj != j) && (ok != j) && (k != j))
            {
                xmi = x - i;
                xpi = x + i;
                if (j > 0)
                {
                    result |= SDL_RenderDrawLine(m_renderer, xmi, y+j, xpi, y+j);  //hline(renderer, xmi, xpi, y + j);
                    result |= SDL_RenderDrawLine(m_renderer, xmi, y-j, xpi, y-j); //hline(renderer, xmi, xpi, y - j);
                }
                else
                {
                    result |= SDL_RenderDrawLine(m_renderer, xmi, y, xpi, y); //hline(renderer, xmi, xpi, y);
                }
                oj = j;
            }

            ix = ix + iy / rx;
            iy = iy - ix / rx;

        }
        while (i > h);
    }
    else
    {
        ix = 0;
        iy = ry * 64;

        do
        {
            h = (ix + 32) >> 6;
            i = (iy + 32) >> 6;
            j = (h * rx) / ry;
            k = (i * rx) / ry;

            if ((oi != i) && (oh != i))
            {
                xmj = x - j;
                xpj = x + j;
                if (i > 0)
                {
                    result |= SDL_RenderDrawLine(m_renderer, xmj, y+i, xpj, y+i); //hline(renderer, xmj, xpj, y + i);
                    result |= SDL_RenderDrawLine(m_renderer, xmj, y-i, xpj, y-i); //hline(renderer, xmj, xpj, y - i);
                }
                else
                {
                    result |= SDL_RenderDrawLine(m_renderer, xmj, y, xpj, y); //hline(renderer, xmj, xpj, y);
                }
                oi = i;
            }
            if ((oh != h) && (oi != h) && (i != h))
            {
                xmk = x - k;
                xpk = x + k;
                if (h > 0)
                {
                    result |= SDL_RenderDrawLine(m_renderer, xmk, y+h, xpk, y+h); //hline(renderer, xmk, xpk, y + h);
                    result |= SDL_RenderDrawLine(m_renderer, xmk, y-h, xpk, y-h); //hline(renderer, xmk, xpk, y - h);
                }
                else
                {
                    result |= SDL_RenderDrawLine(m_renderer, xmk, y, xpk, y); //hline(renderer, xmk, xpk, y);
                }
                oh = h;
            }

            ix = ix + iy / ry;
            iy = iy - ix / ry;

        }
        while (i > h);
    }

    return (result);
}

int pixelRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int result = 0;
    result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
    result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);
    result |= SDL_RenderDrawPoint(renderer, x, y);
    return result;
}

int pixelRGBAWeight(SDL_Renderer * renderer, Sint16 x, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint32 weight)
{
    /*
    * Modify Alpha by weight
    */
    Uint32 ax = a;
    ax = ((ax * weight) >> 8);
    if (ax > 255)
    {
        a = 255;
    }
    else
    {
        a = (Uint8)(ax & 0x000000ff);
    }

    return pixelRGBA(renderer, x, y, r, g, b, a);
}

int aaellipseRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int result;
    int i;
    int a2, b2, ds, dt, dxt, t, s, d;
    Sint16 xp, yp, xs, ys, dyt, od, xx, yy, xc2, yc2;
    float cp;
    double sab;
    Uint8 weight, iweight;

    /*
    * Sanity check radii
    */
    if ((rx < 0) || (ry < 0))
    {
        return (-1);
    }

    /*
    * Special case for rx=0 - draw a vline
    */
    if (rx == 0)
    {
        return SDL_RenderDrawLine(renderer, x, y-ry, x, y+ry); //(vlineRGBA(renderer, x, y - ry, y + ry, r, g, b, a));
    }
    /*
    * Special case for ry=0 - draw an hline
    */
    if (ry == 0)
    {
        return SDL_RenderDrawLine(renderer, x-rx, y, x+rx, y); //(hlineRGBA(renderer, x - rx, x + rx, y, r, g, b, a));
    }

    /* Variable setup */
    a2 = rx * rx;
    b2 = ry * ry;

    ds = 2 * a2;
    dt = 2 * b2;

    xc2 = 2 * x;
    yc2 = 2 * y;

    sab = sqrt((double)(a2 + b2));
    od = (Sint16)lrint(sab*0.01) + 1; /* introduce some overdraw */
    dxt = (Sint16)lrint((double)a2 / sab) + od;

    t = 0;
    s = -2 * a2 * ry;
    d = 0;

    xp = x;
    yp = y - ry;

    /* Draw */
    result = 0;
    result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);

    /* "End points" */
    result |= pixelRGBA(renderer, xp, yp, r, g, b, a);
    result |= pixelRGBA(renderer, xc2 - xp, yp, r, g, b, a);
    result |= pixelRGBA(renderer, xp, yc2 - yp, r, g, b, a);
    result |= pixelRGBA(renderer, xc2 - xp, yc2 - yp, r, g, b, a);

    for (i = 1; i <= dxt; i++)
    {
        xp--;
        d += t - b2;

        if (d >= 0)
            ys = yp - 1;
        else if ((d - s - a2) > 0)
        {
            if ((2 * d - s - a2) >= 0)
                ys = yp + 1;
            else
            {
                ys = yp;
                yp++;
                d -= s + a2;
                s += ds;
            }
        }
        else
        {
            yp++;
            ys = yp + 1;
            d -= s + a2;
            s += ds;
        }

        t -= dt;

        /* Calculate alpha */
        if (s != 0)
        {
            cp = (float) abs(d) / (float) abs(s);
            if (cp > 1.0)
            {
                cp = 1.0;
            }
        }
        else
        {
            cp = 1.0;
        }

        /* Calculate weights */
        weight = (Uint8) (cp * 255);
        iweight = 255 - weight;

        /* Upper half */
        xx = xc2 - xp;
        result |= pixelRGBAWeight(renderer, xp, yp, r, g, b, a, iweight);
        result |= pixelRGBAWeight(renderer, xx, yp, r, g, b, a, iweight);

        result |= pixelRGBAWeight(renderer, xp, ys, r, g, b, a, weight);
        result |= pixelRGBAWeight(renderer, xx, ys, r, g, b, a, weight);

        /* Lower half */
        yy = yc2 - yp;
        result |= pixelRGBAWeight(renderer, xp, yy, r, g, b, a, iweight);
        result |= pixelRGBAWeight(renderer, xx, yy, r, g, b, a, iweight);

        yy = yc2 - ys;
        result |= pixelRGBAWeight(renderer, xp, yy, r, g, b, a, weight);
        result |= pixelRGBAWeight(renderer, xx, yy, r, g, b, a, weight);
    }

    /* Replaces original approximation code dyt = abs(yp - yc); */
    dyt = (Sint16)lrint((double)b2 / sab ) + od;

    for (i = 1; i <= dyt; i++)
    {
        yp++;
        d -= s + a2;

        if (d <= 0)
            xs = xp + 1;
        else if ((d + t - b2) < 0)
        {
            if ((2 * d + t - b2) <= 0)
                xs = xp - 1;
            else
            {
                xs = xp;
                xp--;
                d += t - b2;
                t -= dt;
            }
        }
        else
        {
            xp--;
            xs = xp - 1;
            d += t - b2;
            t -= dt;
        }

        s += ds;

        /* Calculate alpha */
        if (t != 0)
        {
            cp = (float) abs(d) / (float) abs(t);
            if (cp > 1.0)
            {
                cp = 1.0;
            }
        }
        else
        {
            cp = 1.0;
        }

        /* Calculate weight */
        weight = (Uint8) (cp * 255);
        iweight = 255 - weight;

        /* Left half */
        xx = xc2 - xp;
        yy = yc2 - yp;
        result |= pixelRGBAWeight(renderer, xp, yp, r, g, b, a, iweight);
        result |= pixelRGBAWeight(renderer, xx, yp, r, g, b, a, iweight);

        result |= pixelRGBAWeight(renderer, xp, yy, r, g, b, a, iweight);
        result |= pixelRGBAWeight(renderer, xx, yy, r, g, b, a, iweight);

        /* Right half */
        xx = xc2 - xs;
        result |= pixelRGBAWeight(renderer, xs, yp, r, g, b, a, weight);
        result |= pixelRGBAWeight(renderer, xx, yp, r, g, b, a, weight);

        result |= pixelRGBAWeight(renderer, xs, yy, r, g, b, a, weight);
        result |= pixelRGBAWeight(renderer, xx, yy, r, g, b, a, weight);
    }

    return (result);
}

void circle(int xc, int yc, int circleR)
{
    Grapic& g = Grapic::singleton();
    Uint32 cc = *((Uint32*)(&g.getColor()));
    Uint8 *c = (Uint8 *)&cc;
    aaellipseRGBA( g.renderer(), xc, g.inverseY(yc), circleR, circleR, c[0], c[1], c[2], c[3]);
}

void circleFill(int xc, int yc, int circleR)
{
    Grapic& g = Grapic::singleton();
    Uint32 cc = *((Uint32*)(&g.getColor()));
    Uint8 *c = (Uint8 *)&cc;
    filledEllipseRGBA(g.renderer(), xc, g.inverseY(yc), circleR, circleR, c[0], c[1], c[2], c[3]);
}

void put_pixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    Grapic& gr = Grapic::singleton();
    SDL_Color save = gr.getColor();
    color(r,g,b,a);
    SDL_RenderDrawPoint(renderer(), x, gr.inverseY(y));
    color(save.r,save.g,save.b,save.a);
}

void grid(int xmin, int ymin, int xmax, int ymax, int nx, int ny)
{
    int i;
    //Grapic& g = Grapic::singleton();
    float stepx = static_cast<float>(xmax - xmin ) / (nx);
    float stepy = static_cast<float>(ymax - ymin ) / (ny);
    for (i = 0; i <= nx; ++i)
        line(xmin + stepx*i, ymin, xmin + stepx*i, ymax);
    for (i = 0; i <= ny; ++i)
        line(xmin, ymin + stepy*i, xmax, ymin + stepy*i);
}

void mousePos(int& x, int& y)
{
    SDL_PumpEvents();
    SDL_GetMouseState(&x, &y);
    Grapic& g = Grapic::singleton();
    y = g.inverseY(y);
}


void mousePosGlobal(int& x, int& y)
{
    SDL_PumpEvents();
    //SDL_GetMouseState(&x, &y);
//    SDL_GetMouseState(&x, &y);
    SDL_GetGlobalMouseState( &x, &y);
    Grapic& g = Grapic::singleton();
    y = g.inverseY(y);
}


void print(int x, int y, const char* txt)
{
    Grapic& g = Grapic::singleton();
    SDL_Surface* textSurface = TTF_RenderText_Solid( g.font(), txt, g.getColor() );
    SDL_Texture* text = SDL_CreateTextureFromSurface( g.renderer(), textSurface);
    int text_width = textSurface->w;
    int text_height = textSurface->h;
    SDL_FreeSurface(textSurface);
    SDL_Rect renderQuad = { x, g.inverseY(y+text_height), text_width, text_height };
    SDL_RenderCopy( g.renderer(), text, nullptr, &renderQuad);
    SDL_DestroyTexture(text);
}

Uint32 image_get(SDL_Surface *surface, int x, int y)
{
    int nbOctetsParPixel = surface->format->BytesPerPixel;
    y = surface->h-1-y;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;
    switch(nbOctetsParPixel)
    {
    case 1:
        return *p;
    case 2:
        return *(Uint16 *)p;
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
    case 4:
        return *(Uint32 *)p;
    default:
        return 0;
    }
}



//=========================================================================================================================
//=========================================================================================================================
//========================================================================================================================= Image
//=========================================================================================================================
//=========================================================================================================================


Image::Image() : m_surface(nullptr), m_texture(nullptr), m_has_changed(nullptr), nb_instance(nullptr)
{
    cout<<"Image: empty"<<endl;
}


Image::Image(int w, int h) : m_surface(nullptr), m_texture(nullptr), m_has_changed(nullptr), nb_instance(nullptr)
{
    init(w,h);
}


void Image::init(int w, int h)
{
    cout<<"Image: init "<<w<<" "<<h<<endl;
    Grapic& g = Grapic::singleton();
    Uint32 rmask, gmask, bmask, amask;

    destroy();

    int bpp=32;
    SDL_PixelFormatEnumToMasks(g.textureFormat(), &bpp, &rmask, &gmask, &bmask, &amask);
    m_surface = SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, amask); // 0, 0, 0, 0);
    if (m_surface == nullptr)
    {
        printf("error: Can not create this surface: %dx%d\n", w, h);
        return ;
    }

    m_texture = SDL_CreateTextureFromSurface( g.renderer(), m_surface);
    if (m_texture == nullptr)
    {
        printf("error: problem to create the texture\n");
        return ;
    }

    nb_instance = new int;
    *nb_instance = 1;
    m_has_changed = new bool;
    *m_has_changed = false;

    *m_has_changed = false;

    SDL_SetTextureBlendMode( m_texture, SDL_BLENDMODE_BLEND);

    //printInfo();
    //cout<<"Image: init "<<w<<" "<<h<<"  ...ok"<<endl;
}


Image::Image(const char* filename, bool transparency,  unsigned char r, unsigned char g, unsigned b, unsigned char a)
        : m_surface(nullptr), m_texture(nullptr), m_has_changed(nullptr), nb_instance(nullptr)
{
    Grapic& gp = Grapic::singleton();

    m_surface = IMG_Load(filename);
    if (m_surface == nullptr)
    {
        std::string nfn = std::string("../") + filename;
        m_surface = IMG_Load(nfn.c_str());
        if (m_surface == nullptr)
        {
            nfn = std::string("../") + nfn;
            m_surface = IMG_Load(nfn.c_str());
            if (m_surface == nullptr)
            {
                nfn = std::string("../") + nfn;
                m_surface = IMG_Load(nfn.c_str());
            }
        }
    }
    if (m_surface == nullptr)
    {
        std::cout<<"error: Can not load "<< filename<<std::endl;
        return ;
    }
    SDL_Surface * surfaceCorrectPixelFormat = SDL_ConvertSurfaceFormat(m_surface, gp.textureFormat(), 0); //SDL_PIXELFORMAT_RGBA8888,0);
    SDL_FreeSurface(m_surface);
    m_surface = surfaceCorrectPixelFormat;
    if (transparency)
    {
        SDL_Color c;
        c.r=r;
        c.g=g;
        c.b=b;
        c.a=a;
        Uint32 cc = *((Uint32*)(&c));
        Uint32 pixel = SDL_MapRGBA( m_surface->format, r,g,b,a);
        SDL_SetColorKey(m_surface, SDL_TRUE, pixel);
    }
    m_texture = SDL_CreateTextureFromSurface( gp.renderer(), m_surface);

    if (m_texture == nullptr)
    {
        printf("error: problem to create the texture of %s\n", filename);
        SDL_FreeSurface(m_surface);
        return ;
    }
    nb_instance = new int;
    *nb_instance = 1;
    m_has_changed = new bool;
    *m_has_changed = false;
    printf("Image loaded: %s\t", filename);
    printInfo();

}


Image::Image(const Image& im) : m_surface(nullptr), m_texture(nullptr), m_has_changed(nullptr), nb_instance(nullptr)
{
    this->operator=(im);
}


void Image::copy(const Image& im)
{
    destroy();
    cout<<"Image: create from a copy"<<endl;
    init(im.m_surface->w, im.m_surface->h);
    SDL_BlitSurface( im.m_surface, nullptr, m_surface, nullptr);
    *m_has_changed = true;
}


Image::~Image()
{
    destroy();
}


void Image::destroy()
{
    if (nb_instance==nullptr)
    {
        assert(m_surface==nullptr);
        assert(m_texture==nullptr);
        assert(m_has_changed==nullptr);
        return ;
    }

    if (*nb_instance==1)
    {
        SDL_FreeSurface(m_surface);
        SDL_DestroyTexture(m_texture);
        delete nb_instance;
        delete m_has_changed;
        nb_instance = nullptr;
        m_has_changed = nullptr;
        m_surface = nullptr;
        m_texture = nullptr;
//        cout<<"Image::FREE   ";
//        printInfo();
    }
    else
    {
        (*nb_instance)--;
//        cout<<"Image::DETACH   ";
//        printInfo();
        nb_instance = nullptr;
        m_has_changed = nullptr;
        m_surface = nullptr;
        m_texture = nullptr;
    }
}


Image& Image::operator=(const Image& im)
{
    destroy();

    m_surface = im.m_surface;
    m_texture = im.m_texture;
    m_has_changed = im.m_has_changed;
    nb_instance = im.nb_instance;
    if (nb_instance)
        (*nb_instance)++;

//    cout<<"Image::operator= "<<*nb_instance<<" ";
//    printInfo();
//    if (m_surface)
//        SDL_FreeSurface(m_surface);
//    if (m_texture)
//        SDL_DestroyTexture(m_texture);
//
//    init(im.m_surface->w, im.m_surface->h);
//    SDL_BlitSurface( im.m_surface,NULL,m_surface,nullptr);
//    m_has_changed = true;
    return *this;
}


unsigned char Image::get(int x, int y, int c) const
{
    assert(surface());
    SDL_Surface* surf = m_surface;
    if ((x<0) || (y<0) || (x>=surf->w) || (y>=surf->h)) return 0;
    SDL_PixelFormat *fmt = surf->format;
    Uint32 pixel;

    SDL_LockSurface(surf);
    pixel = image_get( surf, x, y);
    SDL_UnlockSurface(surf);

    Uint8 red, green, blue, alpha;
    SDL_GetRGBA( pixel, fmt, &red, &green, &blue, &alpha);

    switch(c)
    {
    case 0: //2
        return red;
    case 1:
        return green;
    case 2: // 0
        return blue;
    case 3:
        return alpha;
    default:
        return red;
    }
}


void image_set(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int nbOctetsParPixel = surface->format->BytesPerPixel;
    y = surface->h-1-y;

    //Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;
    //*((Uint32 *)p) = pixel;

    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;
    switch(nbOctetsParPixel)
    {
    case 1:
        *p = pixel;
        break;
    case 2:
        *(Uint16 *)p = pixel;
        break;
    case 3:
        /*Suivant l'architecture de la machine*/
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        }
        else
        {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;
    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}


void Image::set(int x, int y, unsigned char r, unsigned char g, unsigned b, unsigned char a)
{
    assert(m_surface);
    SDL_PixelFormat *fmt = m_surface->format;
    Uint32 pixel;
    Uint8 *p = (Uint8*)(&pixel);

    if ((x<0) || (y<0) || (x>=m_surface->w) || (y>=m_surface->h)) return;
    SDL_LockSurface(m_surface);
    pixel = SDL_MapRGBA(fmt, r,g,b,a);
    image_set(m_surface, x, y, pixel);
    SDL_UnlockSurface(m_surface);

    *m_has_changed = true;
}


void image_set(Image& im, int x, int y, unsigned char r, unsigned char g, unsigned b, unsigned char a)
{
    im.set(x,y,r,g,b,a);
}


void Image::draw(int x, int y, int w, int h)
{
    assert(m_surface);
    int ok;
    Grapic& g = Grapic::singleton();
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = (w<0)?surface()->w:w;
    r.h = (h<0)?surface()->h:h;
    r.y = g.inverseY( r.y+r.h-1);

    if (*m_has_changed)
    {
        ok = SDL_UpdateTexture(m_texture, nullptr, m_surface->pixels, m_surface->pitch);
        assert(ok == 0);
        *m_has_changed = false;
    }

    ok = SDL_RenderCopy(g.renderer(), m_texture, nullptr, &r);
    assert(ok == 0);
}


void Image::draw(int x, int y, int w, int h, float angle, int flip)
{
    assert(m_surface);
    int ok;
    Grapic& g = Grapic::singleton();
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = (w<0)?m_surface->w:w;
    r.h = (h<0)?m_surface->h:h;
    r.y = g.inverseY( r.y+r.h);

    if (*m_has_changed)
    {
        ok = SDL_UpdateTexture(m_texture, NULL, m_surface->pixels, m_surface->pitch);
        assert(ok == 0);
        *m_has_changed = false;
    }

    SDL_RendererFlip fl;
    switch (flip)
    {
    case 0 :
        fl = SDL_FLIP_NONE;
        break;
    case 1 :
        fl = SDL_FLIP_HORIZONTAL;
        break;
    case 2 :
        fl = SDL_FLIP_VERTICAL;
        break;
    };
    ok = SDL_RenderCopyEx( g.renderer(), m_texture, nullptr, &r, angle, nullptr, fl );
    assert(ok == 0);
}


void Image::savePNG(const char* filename) const
{
    if (!isInit())
    {
        cout<<"Image save: can not save an empty image!"<<endl;
        return;
    }
    Grapic& g = Grapic::singleton();
    SDL_Surface * surfaceCorrectPixelFormat = SDL_ConvertSurfaceFormat( m_surface,g.textureFormat(),0);
    int r = IMG_SavePNG( surfaceCorrectPixelFormat, filename);
    if (r!=0) printf("image: error saving %s\n", filename);
    else printf("Image saved: %s   %dx%d format=%d %s pitch=%d\n", filename, surfaceCorrectPixelFormat->w,
                                                                surfaceCorrectPixelFormat->h,
                                                                surfaceCorrectPixelFormat->format->format,
                                                                SDL_GetPixelFormatName(surfaceCorrectPixelFormat->format->format),
                                                                surfaceCorrectPixelFormat->pitch);
    SDL_FreeSurface(surfaceCorrectPixelFormat);
}


void Image::printInfo() const
{
    if (!isInit())
    {
        printf("imageInfo: not initialized\n");
        return;
    }
    printf("imageInfo: %dx%d format=%d %s pitch=%d",
                                                                m_surface->w,
                                                                m_surface->h,
                                                                m_surface->format->format,
                                                                SDL_GetPixelFormatName(m_surface->format->format),
                                                                m_surface->pitch);
    printf(" surface=%p  texture=%p has_changed=%p  nb_instance=%p", m_surface, m_texture, m_has_changed, nb_instance);
    if (nb_instance) printf(" *nb_instance=%d ", *nb_instance);
    printf("\n");
}



//=========================================================================================================================
//=========================================================================================================================
//========================================================================================================================= MENU
//=========================================================================================================================
//=========================================================================================================================


void Menu::draw(int xmin, int ymin, int xmax, int ymax)
{
    if (m_txt.size()==0) return;
    if (ymax<0) ymax = 25*m_txt.size();
    int i;
    const int fontsize = 12;

    if (isKeyPressed( SDLK_F1))
    {
        m_visible = !m_visible;
        cout<<"menu: "<< (m_visible?"on":"off")<<endl;
    }
    if (!m_visible) return;

    if (xmax<0)
    {
        int nc=-1;
        for(i=0; i<m_txt.size(); ++i)
        {
            if (nc < int(m_txt[i].length()) )
            {
                nc=int(m_txt[i].length());
            }
        }
        xmax = xmin + 0.6f*fontsize*nc + 10;
    }

    if (isMousePressed(SDL_BUTTON_LEFT))
    {
        int x, y;
        mousePos(x, y);
        if ((x>xmin) && (x<xmax) && (y>ymin) && (y<ymax))
        {
            m_select = m_txt.size()-1 - (y-ymin) / ((ymax-ymin)/m_txt.size());
        }
    }

    fontSize(fontsize);                                           // Change the default size of the font
    for(i=0; i<m_txt.size(); ++i)
    {
        if (i==m_select) color(255,55,124);
        else color(255,255,255);
        rectangleFill( xmin, caseToPixel(i,ymin,ymax), xmax, caseToPixel(i+1,ymin,ymax) );

        color(0, 0, 0);                                       // Change the default color (the color of the pen)
        rectangle( xmin, caseToPixel(i,ymin,ymax), xmax, caseToPixel(i+1,ymin,ymax) );

        print(xmin+10, caseToPixel(i+1,ymin,ymax)+(((ymax-ymin)/m_txt.size()) - fontsize)/2 -3,   m_txt[i].c_str());
    }
}



//=========================================================================================================================
//=========================================================================================================================
//========================================================================================================================= PLOT
//=========================================================================================================================
//=========================================================================================================================

struct sort_pred
{
    bool operator() (std::pair<float,float> a, std::pair<float,float> b)
    {
        return ( a.first < b.first );
    }
};

void Plot::add(float x, float y, int curve_n)
{
    if (curve_n<0)
    {
        cerr<<"error==> plot_add: curve number invalid"<<endl;
        return;
    }
    if (curve_n>=m_dat.size()) m_dat.resize(curve_n+1);
    Curve& curve = m_dat[curve_n];
    if ((m_nb_plot_max<0) || (curve.size()<m_nb_plot_max))
    {
        curve.push_back( std::make_pair(x,y) );
        std::sort(curve.begin(), curve.end(), sort_pred() );
    }
    else
    {
        curve.push_back( std::make_pair(x,y) );
        std::sort(curve.begin(), curve.end(), sort_pred() );
        curve.erase( curve.begin() );
    }
}

void Plot::minMax(float& fxmin, float& fymin, float& fxmax, float& fymax, int& maxsize) const
{
    int i,j;
    if (m_dat.size()==0) return;

    fxmin = fxmax = m_dat[0][0].first;
    fymin = fymax = m_dat[0][0].second;
    maxsize = m_dat[0].size();
    for(j=0; j<m_dat.size(); ++j)
    {
        const Curve& cu = m_dat[j];
        if (cu.size()>maxsize) maxsize = cu.size();
        for(i=0; i<cu.size(); ++i)
        {
            if (cu[i].first>fxmax) fxmax = cu[i].first;
            if (cu[i].first<fxmin) fxmin = cu[i].first;
            if (cu[i].second>fymax) fymax = cu[i].second;
            if (cu[i].second<fymin) fymin = cu[i].second;
        }
    }
}

void Plot::draw( const Curve& cu, int xmin, int ymin, int xmax, int ymax, float fxmin, float fymin, float fxmax, float fymax) const
{
    int i;
    float x1, y1, x2, y2;
    if (cu.size()<2) return;
    for(i=0; i<cu.size()-1; ++i)
    {

        x1 = cu[i].first;
        y1 = cu[i].second;
        x2 = cu[i+1].first;
        y2 = cu[i+1].second;

        x1 = xmin + ((x1-fxmin)/(fxmax-fxmin)) * (xmax-xmin);
        x2 = xmin + ((x2-fxmin)/(fxmax-fxmin)) * (xmax-xmin);
        y1 = ymin + ((y1-fymin)/(fymax-fymin)) * (ymax-ymin);
        y2 = ymin + ((y2-fymin)/(fymax-fymin)) * (ymax-ymin);

        line( x1, y1, x2, y2);
    }
}

void Plot::draw(int xmin, int ymin, int xmax, int ymax, bool clearOrNot) const
{
    SDL_Color bg = Grapic::singleton().getBackgroundColor();
    SDL_Color col = Grapic::singleton().getColor();

    float fymin;
    float fymax;
    float fxmin;
    float fxmax;

    if (clearOrNot)
    {
        color( bg.r, bg.g, bg.b);
        rectangleFill( xmin, ymin, xmax, ymax);

        backgroundColor( bg.r, bg.g, bg.b);
        color(col.r, col.g, col.b);
    }

    // Draw axis
    xmin+=25;
    ymin+=20;
    rectangle(xmin,ymin,xmax,ymax);

    //fxmin = cu[0].first;
    //fxmax = cu[ cu.size()-1 ].first;
    int maxsize;
    minMax( fxmin, fymin, fxmax, fymax, maxsize);
    if (m_nb_plot_max>0)
        fxmax = fxmin + ((fxmax-fxmin) * ((float)(m_nb_plot_max))) / maxsize;

    fontSize(11);
    print( xmin-25, ymin-5, fymin);
    print( xmin-25, ymax-15, fymax);

    print( xmin-5, ymin-20, fxmin);
    print( xmax-30, ymin-20, fxmax);

    Grapic& gr = Grapic::singleton();
    SDL_Color save = gr.getColor();
    const int N = 5;
    static const SDL_Color colcu[] = { {0,255,255, 255}, {0,255,0, 255}, {0,0,255, 255}, {255,255,0, 255}, {255,0,255, 255} };
    int i;
    for(i=0; i<m_dat.size(); ++i)
    {
        color( colcu[i%N].r, colcu[i%N].g, colcu[i%N].b, colcu[i%N].a );
        draw( m_dat[i], xmin, ymin, xmax, ymax, fxmin, fymin, fxmax, fymax);
    }
    color(save.r,save.g,save.b,save.a);
}





//=========================================================================================================================
//=========================================================================================================================
//========================================================================================================================= triangle polygon
//=========================================================================================================================
//=========================================================================================================================

void triangle(int x1, int y1,  int x2, int y2, int x3, int y3)
{
    line(x1, y1, x2, y2);
    line(x2, y2, x3, y3);
    line(x3, y3, x1, y1);

}


void regular_polygone(int x, int y, unsigned int apotheme, unsigned int line_number)
{
    //A polygon must have at least 3 vertices
    if(line_number < 3)
    {
        return;
    }

    for(unsigned int i = 0; i < line_number; i++)
    {
        //Simple maths to calculate the position of the points
        line( x + apotheme * cos((2.f * 3.14f *  i     ) / line_number), y + apotheme * sin((2.f * 3.14f *  i     ) / line_number),
              x + apotheme * cos((2.f * 3.14f * (i + 1)) / line_number), y + apotheme * sin((2.f * 3.14f * (i + 1)) / line_number));
    }

}



void triangleFill( int x1, int y1,
                   int x2, int y2,
                   int x3, int y3)
{
#define MAX(a, b) ((a < b) ? b : a)
#define MIN(a, b) ((a < b) ? a : b)

    //Find the maximum Y and the minimum Y in the vertices
    int topY    = MAX( MAX(y1, y2), y3) ;
    int bottomY = MIN( MIN(y1, y2), y3) ;
    //Find the x and the y corresponding to the maximum Y and the minimum Y
    int topX    = (topY    == y1) ? x1 : ( (topY    == y2) ? x2 : x3 ) ;
    int bottomX = (bottomY == y1) ? x1 : ( (bottomY == y2) ? x2 : x3 ) ;

    //Get the last vertex
    int lastX(0), lastY(0);

    if(topX == x1 && topY == y1)
    {
        if(bottomX == x2 && bottomY == y2)
        {
            lastX = x3;
            lastY = y3;
        }
        else
        {
            lastX = x2;
            lastY = y2;
        }
    }
    else if (topX == x2 && topY == y2)
    {
        if(bottomX == x1 && bottomY == y1)
        {
            lastX = x3;
            lastY = y3;
        }
        else
        {
            lastX = x1;
            lastY = y1;
        }
    }
    else if (topX == x3 && topY == y3)
    {
        if(bottomX == x1 && bottomY == y1)
        {
            lastX = x2;
            lastY = y2;
        }
        else
        {
            lastX = x1;
            lastY = y1;
        }
    }
    else
    {
        printf("ERROR : No matching vertex for TOP");
        return;
    }
    //Calculate the linear equations of each segments
    //Prevent from dividing by 0
    float delta = bottomX - topX;
    if(delta == 0)
    {
        delta = 0.0001f;
    }

    const float A = float( bottomY - topY ) / delta;
    const float B = float( bottomX * topY - topX * bottomY ) / delta;

    //Prevent from dividing by 0
    delta = lastX - topX;
    if(delta == 0)
    {
        delta = 0.0001f;
    }

    const float C = float( lastY - topY ) / delta;
    const float D = float( lastX * topY - topX * lastY ) / delta;

    //Prevent from dividing by 0
    delta = lastX - bottomX;
    if(delta == 0)
    {
        delta = 0.0001f;
    }

    const float E = float( lastY - bottomY ) / delta;
    const float F = float( lastX * bottomY - bottomX * lastY ) / delta;

    for( int i = bottomY; i < topY; i++ )
    {
        //Draw lines which are parallel to the x-axis for each pixel on the "main" line delimited by (topX, topY) and (bottomX, bottomY)
        int finalX = ( i >= lastY ) ? ( i - D ) / C : ( i - F ) / E;

        grapic::line(( i - B ) / A, i, finalX, i);
    }
}


void ellipse(int xc, int yc, int h, int v)
{
    Grapic& g = Grapic::singleton();
    Uint32 cc = *((Uint32*)(&g.getColor()));
    Uint8 *c = (Uint8 *)&cc;
    color(c[0], c[1], c[2]);

    aaellipseRGBA(g.renderer(), xc, g.inverseY(yc), h, v, c[0], c[1], c[2], c[3]);
}


void ellipseFill(int xc, int yc, int h, int v)
{
    Grapic& g = Grapic::singleton();
    Uint32 cc = *((Uint32*)(&g.getColor()));
    Uint8 *c = (Uint8 *)&cc;

    filledEllipseRGBA(g.renderer(), xc, g.inverseY(yc), h, v, c[0], c[1], c[2], c[3]);
}


void regular_polygonFill(int x, int y, unsigned int apotheme, unsigned int line_number)
{
    const int MAX_TRIANGLE = 30;

    //A polygon must have at least 3 vertices
    if( line_number < 3)
    {
        return;
    }
    //Draw one triangle is faster than 3
    if( line_number == 3)
    {
        triangleFill(x + apotheme, y,
                     x - apotheme * 0.5, y + apotheme * 0.866,
                     x - apotheme * 0.5, y - apotheme * 0.866);
        return;
    }

    //Above MAX_TRIANGLE vertices the polygon becomes a circle
    if(line_number >= MAX_TRIANGLE)
    {
        circleFill(x, y, apotheme);
        return;
    }

    for(unsigned int i = 0; i < line_number; i++)
    {
        triangleFill(x, y,                                                                                                                  //Point 1 (centre)
                     x + apotheme * cos((2.f * 3.14f *  i     ) / line_number), y + apotheme * sin((2.f * 3.14f *  i     ) / line_number),  //Point 2
                     x + apotheme * cos((2.f * 3.14f * (i + 1)) / line_number), y + apotheme * sin((2.f * 3.14f * (i + 1)) / line_number)); //Point 3
    }
}



bool isInTriangle(float px, float py, float ax, float ay, float bx, float by, float cx, float cy)
{
    //From PolyK.js (http://polyk.ivank.net/)

    float v0x = cx-ax;
    float v0y = cy-ay;
    float v1x = bx-ax;
    float v1y = by-ay;
    float v2x = px-ax;
    float v2y = py-ay;

    float dot00 = v0x*v0x+v0y*v0y;
    float dot01 = v0x*v1x+v0y*v1y;
    float dot02 = v0x*v2x+v0y*v2y;
    float dot11 = v1x*v1x+v1y*v1y;
    float dot12 = v1x*v2x+v1y*v2y;

    float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return (u >= 0) && (v >= 0) && (u + v < 1);
}


void polygonFill(int p[][2], unsigned int number)
{
    //Triangulation Algorithm from PolyK.js (http://polyk.ivank.net/)
    int n = number;
    if (n<3) return;

    std::vector<int> tgs;
    std::vector<int> avl;

    for(int i = 0; i < n; i++)
    {
        avl.push_back(i);
    }

    int i = 0;
    int al = n;

    while(al > 3)
    {

        int i0 = avl[(i + 0)%al];
        int i1 = avl[(i + 1)%al];
        int i2 = avl[(i + 2)%al];

        int ax = p[i0][0];
        int ay = p[i0][1];
        int bx = p[i1][0];
        int by = p[i1][1];
        int cx = p[i2][0];
        int cy = p[i2][1];

        bool earFound = false;
        if((ay-by)*(cx-bx) + (bx-ax)*(cy-by) >= 0)
        {

            earFound = true;

            for(int j = 0; j < al && earFound == true; j++)
            {
                int vi = avl[j];

                if(vi != i0 && vi != i1 &&    vi != i2)
                {
                    if(isInTriangle(p[vi][0], p[ vi][1], ax, ay, bx, by, cx, cy))
                    {
                        earFound = false;
                    }
                }
            }
        }

        if(earFound)
        {
            tgs.push_back(i0);
            tgs.push_back(i1);
            tgs.push_back(i2);

            avl.erase(avl.begin() + ((i + 1) % al));
            al --;
            i = 0;
        }
        else if (i++ > 3 * al)
        {
            break;
        }
    }

    tgs.push_back(avl[0]);
    tgs.push_back(avl[1]);
    tgs.push_back(avl[2]);

    //Draw every triangle from the result of the triangulation
    int x1, y1;
    int x2, y2;
    int x3, y3;

    for(int k = 0; k < tgs.size(); k++)
    {
        if(k % 3 == 0)
        {
            if(k != 0)
            {
                triangleFill(x1, y1, x2, y2, x3, y3);
            }

            x1 = p[tgs[k]][0];
            y1 = p[tgs[k]][1];
        }
        else if(k % 3 == 1)
        {
            x2 = p[tgs[k]][0];
            y2 = p[tgs[k]][1];
        }
        else if(k % 3 == 2)
        {
            x3 = p[tgs[k]][0];
            y3 = p[tgs[k]][1];
        }
    }

    //Draw the last triangle
    triangleFill(x1, y1, x2, y2, x3, y3);

}

void polygon(int p[][2], unsigned int number)
{
    for(unsigned int i = 0; i < number; i++)
    {
        grapic::line(p[i % number][0], p[i % number][1], p[(i + 1) % number][0], p[(i + 1)% number][1]);
    }
}





















} // namespace
