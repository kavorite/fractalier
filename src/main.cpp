#include <iostream>
#include <ctime>
#include <vector>
#include <cstdlib>

#include <png.h>
#include "tclap/CmdLine.h"

#include "colormapping.hpp"
#include "mandelbrot.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    complex<double> anchor, extense;
    unsigned int rWidth, rHeight, depth;
    char* mapFileName = 0;
    char* outFileName = 0;

    const complex<double> anchor_canonical = complex<double>(-2.2, -1.7);
    const complex<double> extense_canonical = complex<double>(3.4, 2.9);

    try {
        stringstream namestrm;
        namestrm << time(0) << ".png"; 

        //Program description, argument delimiter, and version
        TCLAP::CmdLine cmd("Fractalier rendering utility", ' ', "0.1");
 
        TCLAP::ValueArg<double> arg_anchor_real("a", "anchor_real", 
            "'real' value of anchor complex coordinate", false,
            anchor_canonical.real(), "double", NULL);
        TCLAP::ValueArg<double> arg_anchor_imag("s", "anchor_imag",
            "'imaginary' value of anchor complex coordinate", false,
            anchor_canonical.imag(), "double", NULL);
        TCLAP::ValueArg<double> arg_extense_real("e", "extense_real",
            "'real' value of fractal extense", false,
            extense_canonical.real(), "double", NULL);
        TCLAP::ValueArg<double> arg_extense_imag("r", "extense_imag",
            "'imaginary' value of fractal extense", false,
            extense_canonical.imag(), "double", NULL);

        TCLAP::ValueArg<unsigned int> arg_depth("d", "depth",
            "fractal computational depth", false, (unsigned int)255,
            "unsigned integer", NULL);
        TCLAP::ValueArg<unsigned int> arg_height("h", "height",
            "output rendering heighti", true, (unsigned int)768,
            "unsigned integer", NULL);
        TCLAP::ValueArg<unsigned int> arg_width("w", "width",
            "output rendering width", true, (unsigned int)1024,
            "unsigned integer", NULL);
        TCLAP::ValueArg<string> arg_mapfilename("m", "palette",
            "color map file name", true, NULL,
            "string", NULL);
        TCLAP::ValueArg<string> arg_outfilename("o", "output",
            "output file name", false, namestrm.str(),
            "string", NULL);

        cmd.add(arg_anchor_real);
        cmd.add(arg_anchor_imag);
        cmd.add(arg_extense_real);
        cmd.add(arg_extense_imag);

        cmd.add(arg_depth);
        cmd.add(arg_height);
        cmd.add(arg_width);
        cmd.add(arg_mapfilename);
        cmd.add(arg_outfilename);

        cmd.parse(argc, argv);
    } catch (const exception& e) {
        //do stuf
    }


//initial test code
#if 0
    Mandelbrot canonical( std::complex<double>( -2.2, -1.7 ), 
        std::complex<double>( 3.4, 2.9 ), 255 ); 

    auto imageIterations = canonical.render( 1024, 768 );
#endif

#if 0
    for ( unsigned int row = 0; row < imageIterations.size(); row++ ) {
        for ( unsigned int col = 0; col < imageIterations[row].size(); col++ ) {
            printf( "%5d", imageIterations[row][col] );
        }
        printf( "\n" );
    }
#endif

    FractintMapFile m;
    if ( ! m.LoadMapFile( mapFileName ) ) {
        //goto Error;
        return 1;
    }
    png_color palette[256];
    for ( int i = 0; i < 256; i++ ) {
        palette[i].red = m.Map(i).r;
        palette[i].green = m.Map(i).g;
        palette[i].blue = m.Map(i).b;
    }

    Mandelbrot toRender(anchor, extense, depth);
    vector<vector<unsigned int>> renderMap = toRender.render(rWidth, rHeight);
    toRender.WritePNGFile(outFileName, renderMap, (png_colorp)&palette);

#if 0
    for ( unsigned int row = 0; row < imageIterations.size(); row++ ) {
        for ( unsigned int col = 0; col < imageIterations[row].size(); col++ ) {
            printf( "%02X", m.Map(imageIterations[row][col]).b );
        }
        printf( "\n" );
    }
#endif

    return 0;
/*Error:
    return 1;*/
}