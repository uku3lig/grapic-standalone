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

#ifndef GRAPIC_H
#define GRAPIC_H

/// \cond

#include <cassert>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <cmath>
#include <cassert>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

/// \endcond


namespace grapic
{

//! \todo Finish coding the game "dame"



/// \cond

/**
//==================================================================================================
//==================================================================================================
//==================================================================================================
//================================= GRAPIC CLASS ===================================================
//==================================================================================================
//==================================================================================================
//==================================================================================================
* @brief The Grapic class
*/
class Grapic
{
public:
    Grapic();
    void init(const char* name, int w, int h, int posx=-1, int posy=-1, SDL_WindowFlags flag = SDL_WindowFlags(0), std::string render_driver="");
    bool manageEvent();

    void clear();
    void clearEvent();
    bool display();
    void quit();
    void color(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255);
    void colorf(float r, float g, float b, float a=1.f);
    SDL_Color& getColor();
    SDL_Color& getBackgroundColor();
    void backgroundColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255);
    void backgroundColorf(float r, float g, float b, float a=1.f);
    int inverseY(int y);
    void setFont(int s, const char* ttf=NULL);
    int keyHasBeenPressed(unsigned int sc);
    void setKeyRepeatMode(bool kr);

    const SDL_Window* window() const { return m_window; }
    SDL_Window* window() { return m_window; }
    SDL_Renderer * renderer() { return m_renderer; }
    const SDL_Renderer * renderer() const { return m_renderer; }
    TTF_Font* font() { return m_font; }
    const TTF_Font* font() const { return m_font; }
    bool hasFinished() const { return m_quit; }
    bool isInit() const { return m_window; }
    Uint32 textureFormat() const { return m_textureFormat; }
    int winDimX() const { return m_width; }
    int winDimY() const { return m_height; }

    static Grapic& singleton(bool secure=true);

protected:
    int m_width;
    int m_height;
    SDL_Window* m_window;
    SDL_Renderer *m_renderer;
    SDL_RendererInfo m_renderInfo;
    Uint32 m_textureFormat;
    TTF_Font *m_font;
    std::string m_fontFile;
    int m_fontSize;
    bool m_quit;
    bool m_anim;
    SDL_Color m_currentColor;
    SDL_Color m_backgroundColor;
    std::vector<int> m_keyStates;
    bool m_keyRepeatMode;
    int imagesSavedCount;

    void initKeyArray();
    void help() const;
    bool manageOneEvent(SDL_Event event);

    static Grapic* currentGrapic;
    friend void winInit(const char* name, int w, int h, int posx, int posy);
    friend void winQuit();
};




class Image
{
public:
    Image();
    Image(const char* filename, bool transparency, unsigned char r, unsigned char g, unsigned b, unsigned char a);
    Image(const Image& im);
    Image(int w, int h);
    ~Image();

    Image& operator=(const Image& im);

    void copy(const Image& im);
    void savePNG(const char* filename) const;
    bool isInit() const;
    unsigned char get(int x, int y, int c) const;
    void set(int x, int y, unsigned char r, unsigned char g, unsigned b, unsigned char a);
    void printInfo() const;

    const SDL_Surface* surface() const;

    void draw(int x, int y, int w, int h);
    void draw(int x, int y, int w, int h, float angle, int flip);

protected:
    SDL_Surface* m_surface;
    SDL_Texture* m_texture;
    bool* m_has_changed;
    int* nb_instance;

    void init(int w, int h);
    void destroy();
};





class Menu
{
public:
    Menu();
    void change(int i, const std::string& str);
    int select() const;
    void setSelect(int s);
    int caseToPixel(int c, int ymin, int ymax) const;
    void draw(int xmin, int ymin, int xmax, int ymax);
    void add(const std::string& str);

protected:
    std::vector<std::string> m_txt;
    int m_select;
    bool m_visible;
};



typedef std::vector< std::pair<float,float> > Curve;
class Plot
{
public:
    Plot();
    void clear();
    void setSize(const int n);
    void add(float x, float y, int curve_n);
    void draw(int xmin, int ymin, int xmax, int ymax, bool clearOrNot) const;
    void draw( const Curve& cu, int xmin, int ymin, int xmax, int ymax, float fxmin, float fymin, float fxmax, float fymax) const;
    void minMax(float& fxmin, float& fymin, float& fxmax, float& fymax, int& maxsize) const;

protected:
    std::vector< Curve  > m_dat;
    int m_nb_plot_max;
};



/// \endcond






//==================================================================================================
//==================================================================================================
//==================================================================================================
//============================= GRAPIC FUNCTIONS ===================================================
//==================================================================================================
//==================================================================================================
//==================================================================================================




/** \brief Initialize the window with a size w,h and a position (posx,posy).
    If posx<0 or posy<0, the position is centered.
*/
inline void winInit(const char* name, int w, int h, int posx=-1, int posy=-1)
{
    Grapic::currentGrapic = new Grapic();
    Grapic::singleton(false).init(name,w,h,posx,posy);
}

/** \brief Clear the window with the default background color
    ~~~~~~~~~~~~~~~{.c}
	bool stop=false;
	winInit("Tutorials", 500, 500);
	winClear();
	rectangle(100,100, 200, 200);
	winDisplay();
	pressSpace();
	winQuit();
    ~~~~~~~~~~~~~~~
*/
inline void winClear()
{
    grapic::Grapic::singleton().clear();
}

/** \brief return true if the application should be closed (because of 'ESC' or 'q' key pressed for instance)
*/
inline bool winHasFinished()
{
    return Grapic::singleton().hasFinished();
}

/** \brief Clear the event queue of the window
*/
inline void winClearEvent()
{
    grapic::Grapic::singleton().clearEvent();
}

/** \brief Change the size (w,h), the position(ps,py) or the fullscreen on/off
     Set a negative parameter to let him as it is.
*/
void winSetPosition(int w, int h, int ps, int py, bool fullscreen);


/** \brief Display the window. All drawing is hidden until this function is not called.
    ~~~~~~~~~~~~~~~{.c}
	bool stop=false;
	winInit("Tutorials", 500, 500);
	while( !stop )
	{
		winClear();
		draw();
		stop = winDisplay();
	}
	winQuit();
    ~~~~~~~~~~~~~~~
*/
inline bool winDisplay()
{
    return Grapic::singleton().display();
}

/** \brief Quit and close all things
*/
inline void winQuit()
{
    Grapic::singleton().quit();
    if (Grapic::currentGrapic)
    {
        delete Grapic::currentGrapic;
        Grapic::currentGrapic = nullptr;
    }
}

/** \brief Change the default color (unsigned char values between 0 and 255)
*/
inline void color(unsigned char _r = 255, unsigned char _g = 255, unsigned char _b = 255, unsigned char _a = 255)
{
    Grapic::singleton().color( _r, _g, _b, _a );
}

/** \brief Change the default color (float values between 0.f and 1.f)
*/
inline void colorf(float _r = 1.f, float _g = 1.f, float _b = 1.f, float _a = 1.f)
{
    Grapic::singleton().colorf( _r, _g, _b, _a );
}

/** \brief Change the default background color (the color used to clear the screen)
*/
inline void backgroundColor(unsigned char _r = 255, unsigned char _g = 255, unsigned char _b = 255, unsigned char _a = 255)
{
    Grapic::singleton().backgroundColor( _r, _g, _b, _a);
}

/** \brief Change the default background color (the color used to clear the screen)
*/
inline void backgroundColorf(float _r = 1.f, float _g = 1.f, float _b = 1.f, float _a = 1.f)
{
    Grapic::singleton().backgroundColorf( _r, _g, _b, _a);
}

/** \brief Draw a circle from (xmin,ymin) to (xmax,ymax)
*/
void circle(int xc, int yc, int radius);

/** \brief Draw a filled circle from (xmin,ymin) to (xmax,ymax)
*/
void circleFill(int xc, int yc, int radius);

/** \brief Draw an ellipse at (xc, yc), horizontal radius and vertical radius, rotation optional (Thanks to Anass LAHNIN)
*/
void ellipse(int xc, int yc, int horizontal, int vertical);

/** \brief Draw a filled ellipse at (xc, yc), horizontal radius and vertical radius, rotation optional  (a bit long when drawing at angle != {90*k, k∈ℤ})
*/
void ellipseFill(int xc, int yc, int horizontal, int vertical);

/** \brief Draw a rectangle from (xmin,ymin) to (xmax,ymax)
*/
void rectangle(int xmin, int ymin, int xmax, int ymax);

/** \brief Draw a filled rectangle from (xmin,ymin) to (xmax,ymax)
*/
void rectangleFill(int xmin, int ymin, int xmax, int ymax);

/** \brief Draw a line from (x1,y1) to (x2,y2)
*/
inline void line(int x1, int y1, int x2, int y2)
{
    Grapic& g = Grapic::singleton();
    SDL_RenderDrawLine( g.renderer(), x1, g.inverseY(y1), x2, g.inverseY(y2));
}

/** \brief Draw a pixel on (x,y) with color (r,g,b,a)
*/
void put_pixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a=255);

/** \brief Draw a point at (x,y)
*/
inline void point(int x, int y)
{
    Grapic& g = Grapic::singleton();
    SDL_RenderDrawPoint( g.renderer(), x, g.inverseY(y));
}

/** \brief Draw an array of n points
*/
inline void points(int p[][2], int n)
{
    Grapic& g = Grapic::singleton();
    SDL_RenderDrawPoints( g.renderer(), ((const SDL_Point*)(p)), n);
}

/** \brief Draw a grid from (xmin,ymin) to (xmax,ymax) with nx columns and ny rows
*/
void grid(int xmin, int ymin, int xmax, int ymax, int nx, int ny);

/** \brief return a random number (integer) between rmin to rmax included
*/
inline int irand(int rmin=0, int rmax=100)
{
    return rmin + rand() % (rmax - rmin + 1);
}

/** \brief return a random number (float) between rmin to rmax included
*/
inline float frand(float rmin = 0.f, float rmax = 1.f)
{
    float r = static_cast<float>(rand()) / RAND_MAX;
    return rmin + r * (rmax - rmin);
}

/** \brief return the time elapsed since the beginning of the process in second
*/
inline float elapsedTime()
{
    return 0.001f * SDL_GetTicks();
}

/** \brief return the number of time the key 'key' has been pressed since the last call to this function.
    ~~~~~~~~~~~~~~~{.c}
	if (isKeyPressed(SDLK_LEFT)) { ... }     // test if the left arrow is pressed
	if (isKeyPressed(SDLK_LEFT)) {  ... }    // if the left arrow is pressed then do ...
	if (isKeyPressed(SDLK_a)) {  ... }       // if the key 'a' is pressed then do ...
	if (isKeyPressed('a')) { ... }          // if the key 'a' is pressed then do ...
    ~~~~~~~~~~~~~~~
*/
inline int isKeyPressed(int key)
{
    Grapic& g = Grapic::singleton();
    return (g.keyHasBeenPressed(key)); // || (state[key]>0) );
}


/** \brief (de)Activate the repeat mode: when the user presses continuously on the key touch is repeated. It has to be set at the beguinning of the program.
*/
inline void setKeyRepeatMode(bool repeat)
{
    Grapic& g = Grapic::singleton();
    g.setKeyRepeatMode(repeat);
}


/** \brief Stop the program during d milliseconds
    ~~~~~~~~~~~~~~~{.c}
	if (isKeyPressed(SDLK_LEFT)) {  ... }    // if the left arrow is pressed then do ...
	if (isKeyPressed(SDLK_a)) {  ... }       // if the key 'a' is pressed then do ...
	if (isKeyPressed('a')) { ... }          // if the key 'a' is pressed then do ...
    ~~~~~~~~~~~~~~~
*/
inline void delay(int d)
{
    SDL_Delay(d);
}

/** \brief return true if the mouse button 'button' is pressed
    ~~~~~~~~~~~~~~~{.c}
    if (isMousePressed(SDL_BUTTON_LEFT)) { ... }    // test if the left button of the mouse is pressed
    ~~~~~~~~~~~~~~~
*/
inline bool isMousePressed(int button)
{
    return SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(button);
}

/** \brief After this function (x,y) store the mouse position
    ~~~~~~~~~~~~~~~{.c}
    int x,y;
    mousePos(x,y);                                                                          // test if the left button of the mouse is pressed
    if ((isMousePressed(SDL_BUTTON_LEFT)) && (x>10) && (y>10) && (x<50) && (y<50)) { ... }  // if the user clicks if the square (10,10)(50,50)
    ~~~~~~~~~~~~~~~
*/
void mousePos(int& x, int& y);

/** \brief After this function (x,y) store the mouse position
    ~~~~~~~~~~~~~~~{.c}
    int x,y;
    mousePosGlobal(x,y);                                                                          // test if the left button of the mouse is pressed
    if ((isMousePressed(SDL_BUTTON_LEFT)) && (x>10) && (y>10) && (x<50) && (y<50)) { ... }  // if the user clicks if the square (10,10)(50,50)
    ~~~~~~~~~~~~~~~
*/
void mousePosGlobal(int& x, int& y);


/** \brief Manage standard event like key 'ESC', quit, etc.
*/
inline bool winManageEvent()
{
    return Grapic::singleton().manageEvent();
}

/** \brief Change the default size of the font
*/
inline void fontSize(int s)
{
    Grapic::singleton().setFont(s);
}


/** \brief Print the text txt , up left corner is (x,y)
    ~~~~~~~~~~~~~~~{.c}
    fontSize(24);                               // Change the default font to the size 24
    print( 10, 20, "Bonjour");                  // Print the text "Bonjour" with the left corner (10,20)
    ~~~~~~~~~~~~~~~
*/
void print(int x, int y, const char* txt);

/** \brief Print the integer nb, up left corner is (x,y)
    ~~~~~~~~~~~~~~~{.c}
    print( 10, 20, 128);                      // Print the number "128" with the left corner (10,20)
    ~~~~~~~~~~~~~~~
*/
inline void print(int x, int y, int nb)
{
    char txt[64];
    sprintf(txt,"%d", nb);
    print(x,y,txt);
}


/** \brief Print the float nb, up left corner is (x,y)
    ~~~~~~~~~~~~~~~{.c}
    print( 10, 20, 3.1415);                      // Print the number "3.1415" with the left corner (10,20)
    ~~~~~~~~~~~~~~~
*/
inline void print(int x, int y, float nb)
{
    char txt[64];
    sprintf(txt,"%.2f", nb);
    print(x,y,txt);
}

/** \brief Stop the program until key 'space'is pressed
*/
void pressSpace(bool isPrint=true);




/** \brief Return an image loaded from the file filename
    ~~~~~~~~~~~~~~~{.c}
    struct Data
    {
        Image im;
    };
    void init(Data& d)
    {
        d.im = image("data/grapic.bmp", true, 255, 255, 255, 255);      // load the image "data/grapic.bmp" with transparency for the color (255,255,255,255)
    }
    void draw(Data& d)
    {
        image_draw(d.im, 0, 255);                                       // Draw the image with the left corner (0,255)
    }
    ~~~~~~~~~~~~~~~
*/
inline Image image(const char* filename, bool transparency=false, unsigned char r=255, unsigned char g=255, unsigned b=255, unsigned char a=255)
{
    return Image(filename, transparency, r, g, b, a);
}


/** \brief Return an image of width=w and height=h
*/
inline Image image(int w, int h)
{
    return Image(w,h);
}

/** \brief Return a copy of the image. It duplicates the image. It is useful since an affectation shares the same image.
*/
Image image_copy(const Image& im);

/** \brief Save the image into the file: format is PNG
*/
inline void image_savePNG(const Image& im, const char* filename)
{
    im.savePNG(filename);
}

/** \brief Draw the image at position (x,y) with width=w and height=h (if w<0 or h<0 the original size of the image is used)
*/
inline void image_draw(Image& im, int x, int y, int w=-1, int h=-1)
{
    im.draw(x,y,w,h);
}

/** \brief Draw the image at position (x,y) with width=w and height=h (if w<0 or h<0 the original size of the image is used); angle indicate the angle of rotation and flip: 0=no flip, 1=horizontal flip, 2=vertical flip
*/
inline void image_draw(Image& im, int x, int y, int w, int h, float angle, float flip=SDL_FLIP_NONE)
{
    im.draw(x,y,w,h,angle,flip);
}

/** \brief return the color component c of the pixel (x,y) of the image im.
    c must be 0 for the red component, 1 for the green component, 2 for the blue component or 3 for the alpha/opacity component.
*/
inline unsigned char image_get(const Image& im, int x, int y, int c=0)
{
    return im.get(x,y,c);
}

/** \brief Set the pixel (x,y) of the image im with the color c
*/
void image_set(Image& im, int x, int y, unsigned char r, unsigned char g, unsigned b, unsigned char a);


/** \brief return the width of the image
*/
inline int image_width(const Image& im)
{
    return im.surface()->w;
}


/** \brief return the height of the image
*/
inline int image_height(const Image& im)
{
    return im.surface()->h;
}

/** \brief return true if the image is initialized
    ~~~~~~~~~~~~~~~{.c}
	d.im = image("data/grapic.bmp", true, 255, 255, 255, 255);
    if (!image_isInit(d.im))                                            // if the image was not loaded, try an other path
        d.im = image("../data/grapic.bmp", true, 255,255,255,255 );
    ~~~~~~~~~~~~~~~
*/
inline bool image_isInit(const Image& im)
{
    return im.isInit();
}

/** \brief Print the informations of the image im
*/
inline void image_printInfo(const Image& im)
{
    im.printInfo();
}


/** \brief Add a line to the menu m with the text str
    ~~~~~~~~~~~~~~~{.c}
	Menu m;
	bool stop=false;
	winInit("Tutorials", 500, 500);
	menu_add( m, "Question 1");
	menu_add( m, "Question 2");
	while( !stop )
	{
		winClear();
		menu_draw(m, 5,5, 100, 102);
		switch(menu_select(m))
		{
			case 0 : draw1(); break;
			case 1 : draw2(); break;
		}
		stop = winDisplay();
	}
	winQuit();
    ~~~~~~~~~~~~~~~
*/
inline void menu_add(Menu& m, const std::string& str)
{
    m.add(str);
}


//! \brief Change the text of a line in the menu
inline void menu_change(Menu& m, int i, const std::string& str)
{
    m.change(i,str);
}

//! \brief Draw the menu on the screen. See menu_add for an example of usage.
inline void menu_draw(Menu& m, int xmin=5, int ymin=5, int xmax=-1, int ymax=-1)
{
    m.draw(xmin,ymin,xmax,ymax);
}

//! \brief return the line selected in the menu. See menu_add for an example of usage.
inline int menu_select(const Menu& m)
{
    return m.select();
}

//! \brief return the line selected in the menu. See menu_add for an example of usage.
inline void menu_setSelect(Menu& m, int s)
{
    m.setSelect(s);
}

//! \brief return the pixel from a line of the menu
inline int caseToPixel(const Menu& m, int c, int ymin, int ymax)
{
    return m.caseToPixel(c,ymin,ymax);
}

//! @todo: plot: setColor for each curves
//! @todo: setRangeXMinMax for each curves

//! \brief Clear the data stored
inline void plot_clear(Plot& p )
{
    p.clear();
}

//! \brief Define the size of the stored value of the funtion (<0 means infinity)
inline void plot_setSize(Plot& p, const int n)
{
    p.setSize(n);
}

//! \brief Add a point (x,y=f(x)) to the curve number curve_n
inline void plot_add(Plot& p, float x, float y, int curve_n=0)
{
    p.add(x,y,curve_n);
}

//! \brief Draw the curve in the rectangle (xmin,ymin,xmax,ymax); clear the rectangle if clearOrNot is true
inline void plot_draw( const Plot& p, int xmin, int ymin, int xmax, int ymax, bool clearOrNot=true)
{
    p.draw(xmin,ymin,xmax,ymax,clearOrNot);
}




/**
    \brief Draw a triangle from the vertices (x1, y1), (x2, y2) and (x3, y3). (Code provided by Bastien DOIGNIES, many thanks)
 */
void triangle(int x1, int y1,  int x2, int y2, int x3, int y3);

/**
    \brief Draw a regular polygon with line_numbers edges centred on (x, y). (Code provided by Bastien DOIGNIES, many thanks)
 */
void regular_polygone(int x, int y, unsigned int apotheme, unsigned int line_number);

/**
    \brief Draw a filled triangle from the vertices (x1, y1), (x2, y2), (x3, y3)
 */
void triangleFill( int x1, int y1,
                   int x2, int y2,
                   int x3, int y3);


/**
    \brief Draw a filled regular polygon with line_numbers edges centred on (x, y). (Code provided by Bastien DOIGNIES, many thanks)
*/
void regular_polygonFill(int x, int y, unsigned int apotheme, unsigned int line_number);


/**
    \brief Decide if a point (px, py) is inside the triangle (ax, ay), (bx, by), (cx, xy). (Code provided by Bastien DOIGNIES, many thanks)
    \return true if inside, false otherwise
 */
bool isInTriangle(float px, float py, float ax, float ay, float bx, float by, float cx, float cy);


/** \brief  Draw a simple(no edge-crossing) and without holes filled polygon
 * \param   p[]    : array of integer which represent the coordinate of each vertices
 * \param   number : number of element in p[], must be even (point are two integers each)!
 * \warning Draw nothing if there are not enough vertices or if number is not even
 * \warning Undetermined draw if the polygon is not simple
 (Code provided by Bastien DOIGNIES, many thanks)
    ~~~~~~~~~~~~~~~{.c}
    int p[4][2];
    p[0][0] = 20;       p[0][1] = 20;       // sommet 0, coord X et Y
    p[1][0] = 200;      p[1][1] = 20;       // sommet 1, coord X et Y
    p[2][0] = 200;      p[2][1] = 200;      // sommet 2, coord X et Y
    p[3][0] = 20;       p[3][1] = 200;      // sommet 3, coord X et Y
    polygonFill(p, 4);
    ~~~~~~~~~~~~~~~
 */
void polygonFill(int p[][2], unsigned int number);


/** \brief Draw a polygon. (Code provided by Bastien DOIGNIES, many thanks)
    ~~~~~~~~~~~~~~~{.c}
    int p[4][2];
    p[0][0] = 20;       p[0][1] = 20;       // sommet 0, coord X et Y
    p[1][0] = 200;      p[1][1] = 20;       // sommet 1, coord X et Y
    p[2][0] = 200;      p[2][1] = 200;      // sommet 2, coord X et Y
    p[3][0] = 20;       p[3][1] = 200;      // sommet 3, coord X et Y
    polygon(p, 4);
    ~~~~~~~~~~~~~~~
 */
void polygon(int p[][2], unsigned int number);





} // namespace

#endif







//==================================================================================================
//==================================================================================================
//==================================================================================================
//============================= DOCUMENTATION MAIN PAGE ============================================
//==================================================================================================
//==================================================================================================
//==================================================================================================

/*! \mainpage GrAPiC

<br>
\image html grapic_samples.jpg
<br>
\section intro Introduction

GrAPiC is a small, easy-to-use framework to add graphical functionalities to your C/C++ code. Its main goal is to provide easy
graphical functions to be used in introduction courses of algorithm and programming in C/C++.
It is based on SDL2 but to use it you need to know only few very simples functions.
The Grapic archive comes with the needed SDL part, you do not have to install SDL. Just unzip and enjoy !
<br>
<br>
<br>

\section download Download
Download and install the last version:\htmlinclude "VERSION.html"
Previous versions may be downloaded from the [download](../download) directory.

Or you can clone a version from the GitHub repo : [https://github.com/ucacaxm/grapic](https://github.com/ucacaxm/grapic)

<br>
<br>
<br>
\section start How to start ?

\subsection win Run on Windows with Codeblocks (the default)
  - Install Codeblocks 20.03 (IMPORTANT) from: [http://www.codeblocks.org/downloads](http://www.codeblocks.org/downloads)\n
    Take the one with MinGW like codeblocks-20.03mingw-setup.exe

  - Download the archive file, see section download.

  - Run with CodeBlocks
     - Open the file Grapic/build/windows/grapic_workspace with Codeblocks
     - Select in the projects manager (on the left) the project you want with the right button "Activate project":
       \image html cb_activatePrj.jpg "Selection of the project in Codeblocks" width=400
     - And run the program !
\\
\\
\subsection winVSCode Run on Windows with Visual Code/VSCode (mostly for advanced users).
  - VSCode is not Visual Studio
  - Install MinGW as described here:
     - [Using GCC with MinGW for Visual Studio Code on Windows](https://code.visualstudio.com/docs/cpp/config-mingw) \n
     - Add the path to your Mingw-w64 bin folder to the Windows PATH environment variable by using the following steps: \n
       In the Windows search bar, type 'settings' to open your Windows Settings. \n
          - Search for Edit environment variables for your account. \n
          - Choose the Path variable and then select Edit. \n
          - Select New and add the Mingw-w64 destination folder path to the system path. The exact path depends on which version of Mingw-w64 you have installed and where you installed it. If you used the settings above to install Mingw-w64, then add this to the path: `C:\msys64\mingw64\bin`. \n
          - Select OK to save the updated PATH. You will need to reopen any console windows for the new PATH location to be available \n
     - [Install C++ extension for VSCode](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)\\
  - Compile and Run: first option with the file tasks.json
     - Open Visual Studio Code, then menu File+Open Folder and select grapic.code-workspace in the the grapic folder.\n
     - Compile/Build: Ctrl+Shift+B. It calls the makefile and compile the default file which is apps/start/main_start.cpp.
     - Run/Test: Ctrl+Shift+P and enter `run test`. It runs the default executable: bin\start.exe. You can add a shortcut.
     - Edit the file `.vscode/tasks.json`to change the project
  - Compile and Run: second option with the terminal
     - Open a terminal (Ctrl+ù or menu Terminal/new terminal)\n
     - Test if the installation of MinGW and the path setting are OK: run `g++` and `mingw32-make` just to see if ok\n
     - Change directory (and stay always in this directory): `cd build\\windows-cb20`
     - To compile: `mingw32-make -f Start.make`
     - To run: `..\\..\\bin\\Start.exe`
  - Compile and Run: third option with CMake
     - install cmake support for Visual Code and open script/cmake/CMakeLists.txt


\subsection linux Run on Linux
  - Install SDL2, SDL_Image for SDL2 and SDL_ttf for SDL2 ; for instance with
    \code
    sudo sudo apt install g++ premake4 zip doxygen libsdl2-dev libsdl2-mixer-dev libsdl2-image-dev libsdl2-ttf-dev
    \endcode

  - Download the archive file, see section download.

  - Run with CodeBlocks
     - Open the file Grapic/build/linux/grapic.workspace with Codeblocks.
     - Select in the projects manager (on the left) the project you want with the right button "Activate project":
       \image html cb_activatePrj.jpg "Selection of the project in Codeblocks" width=400
     - And run the program !
  - Compile all and run in the Terminal:
     - cd ....grapic
     - make
     - bin/demo
  - Compile one program and run in the Terminal:
     - cd ....grapic
     - make projectName    (replace projectName by the name of your project: 'make list' provides the list of all projects)
     - bin/projectName


\subsection mac Run on MacOS
  - Update MacOS to the last version

  - Update XCode
    - Open App Store
    - Look in the top right for the updates section
    - Find Xcode & click Update

  - <b>If you have a Mac M1 (ARM processor ARM, not an INTEL), you need Rosetta 2</b>
    - In a terminal, run this: `softwareupdate --install-rosetta`
    - Or see: https://infoidevice.fr/comment-installer-rosetta-2-sur-mac-apple-silicon/

  - Install grapic https://licence-infi.univ-lyon1.fr/grapic
    - Download the zip file for MacOS (grapic-V.V.VV-MacOS.zip)
    - Unzip it

  - With XCode
    - Run a terminal this command to launch XCode, or launch XCode from the menu
      \code
      xcode-select --install
      \endcode
    - Open a project in this path: grapic-V.V.VV-macosx/build/macosx/Start.xcodeproj
    - Compile and run


  - Run in a terminal (bash)
    - Enter these commands to compile:
        \code
        cd .../grapic-V.V.VV-MacOS          # trouver le chemin vers l'endroit où vous avez décompressé le .zip
        make
        \endcode
    - To run: `bin/Start`
    - It should open the grapic app with squares




\subsection replit Run on Replit (web without any installation)
  - You can try Grapic on the web with Replit: https://repl.it/@ucax217/grapic-light#main.cpp
  - Create an account on repli.it
  - Fork the project to be abble to change it
  - All the code is in the file main.cpp
  - "run": compilation errors are in the terminal on the right. Execution is on the top right.



\subsection edit Edit my first program
The kernel source code of grapic is in grapic/src. The source codes of the programs/demos/tutorials are in grapic/apps.
You can edit your first program by editing the file grapic/apps/start/main_start.cpp
\image html cb_editStart.jpg width=10


\subsection create Create a new project
You can create your own project.
 - Copy the directory apps/start into apps/MYPROJECT  (replace MYPROJECT by the name you want)
 - Rename apps/MYPROJECT/main_start.cpp into main_MYPROJECT.cpp
 - Edit the file Grapic/premake4.lua and add at the end
make_project( "MYPROJECT", 	"apps/MYPROJECT/main_MYPROJECT.cpp" )
 - Run the script 'premake.bat' on windows; or 'make premake' on Linux/MacOS

If you are following the course LIFAMI, run lifami.bat on windows or enter "make lifami" on Linux in the command line, it will create for you all the projects for the tutorials.




<br>
<br>
<br>
\section tuto Tutorials

<br>
<br>
<br>
\subsection tuto1 Tutorial 1: Simple program
\image html tuto_simple.jpg "The code of the tutorial 1 "simpleDrawing" illustrates the basis of Grapic" width=200px
\include "../apps/tutorials/tuto1_Simple.cpp"

<br>
<br>
<br>
\subsection tuto2 Tutorial 2: First Loop
\image html tuto_drawing.jpg "The code of the tutorial 2 shows how to write the main loop of a Grapic application" width=200px
\include "../apps/tutorials/tuto2_FirstLoop.cpp"

<br>
<br>
<br>
\subsection tuto3 Tutorial 3: Drawing
\image html tuto_drawing.jpg "The code of the tutorial 3 shows how to draw rectangles, circles, lines, etc." width=200px
\include "../apps/tutorials/tuto3_Drawing.cpp"

<br>
<br>
<br>
\subsection tuto4 Tutorial 4: Keyboard
\image html tuto_drawing.jpg "The code of the tutorial 4 shows how to manage the keyboard." width=200px
\include "../apps/tutorials/tuto4_Keyboard.cpp"

<br>
<br>
<br>
\subsection tuto5 Tutorial 5: Mouse
\image html tuto_mouse.jpg "The code of the tutorial 5 shows how manage the mouse to draw a square" width=200px
\include "../apps/tutorials/tuto5_Mouse.cpp"

<br>
<br>
<br>
\subsection tuto6 Tutorial 6: Menu
\image html tuto_menu.jpg "The code of the tutorial 6 shows how to create a simple menu." width=200px
\include "../apps/tutorials/tuto6_Menu.cpp"

<br>
<br>
<br>
\subsection tuto7 Tutorial 7: Animation
\image html tuto_animation.jpg "The code of the tutorial 2 shows how to display a simple animation depending on the elapsed time." width=200px
\include "../apps/tutorials/tuto7_Animation.cpp"

<br>
<br>
<br>
\subsection tuto8 Tutorial 8: Plot/Graph
\image html tuto_plot.jpg "The plot code illustrates the drawing of a graph entering values of a function" width=200px
\include "../apps/tutorials/tuto8_Plot.cpp"

<br>
<br>
<br>
\subsection tuto9 Tutorial 9: Image
\image html tuto_image.jpg "The demo code illustrates image functionalities of Grapic" width=200px
\include "../apps/tutorials/tuto9_Image.cpp"


<br>
<br>
<br>
\subsection tuto10 Tutorial 10: Demo
\image html tuto_demo.jpg "The demo code illustrates many functionalities of Grapic" width=200px
\include "../apps/tutorials/tuto10_Demo.cpp"


<br>
<br>
<br>
\section examples Examples

Grapic comes with several small examples. It can be used as source of inspiration.


\subsection tetris Tetris
\image html tetris.jpg "A example of tetris" width=200px
\include "../apps/demo_tetris/main_tetris.cpp"


\subsection pacman Pacman
\image html pacman.jpg "Pacman" width=200px
\include "../apps/demo_pacman/main_pacman.cpp"



<br>
<br>
\section questions Questions and problems

You can check and ask your question in the issues section of GitHub : [https://github.com/ucacaxm/grapic/issues](https://github.com/ucacaxm/grapic/issues)


- int main() ==> int main(int , char **)

- Image im = image("data\papillon.jpg") ==> Image im = image("data//papillon.jpg")   or Image im = image("data\\papillon.jpg")

- Even tutorials do not compile
   - check in directory 'extern' that you have the version for your computer: mingw for windows, macox for mac and nothing for
        linux since you have to install them with the apt explained in the Section installtion
   - Try to rerun premake with premake.bat on windows or make premake on linux/macos

*/
