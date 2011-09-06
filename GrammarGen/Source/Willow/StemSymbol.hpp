//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _STEM_SYMBOL_HPP_
#define _STEM_SYMBOL_HPP_

#include "Arrays.hpp"
#include "Distributions.hpp"
#include "Symbol.hpp"
#include "Width.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class StemSymbol
//#####################################################################
class StemSymbol : public Symbol
{
public:
    StemSymbol(Symbol** symbolTable)
     :Symbol(true,1,5,0,4,symbolTable)
    {}

    StemSymbol(const bool isTerminal,const uint maxNumSymbols,const uint maxNumSymbolEntries,const uint numDiffusionParams,const uint numFormalParams,Symbol** symbolTable)
     :Symbol(isTerminal,maxNumSymbols,maxNumSymbolEntries,numDiffusionParams,numFormalParams,symbolTable)
    {}

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        output[0].mID=202;
        memcpy(output+1,input,sizeof(SymbolEntry)*4);
        logProbability=0;
        return 5;
    }

    virtual ushort setFlags(const float* params) const
    {
        return SymbolEntry::DETERMINISTIC;
    }

    virtual void render(const float* params) const
    {
        mResourceHandler->bindShader(mShader);
		float width_parameter = getWidth();

		glBegin (GL_TRIANGLE_STRIP);
#define NUM_FACES 10

		for (int i = 0; i <= NUM_FACES; i++)
		{
			float angle = ((float)i / (float)NUM_FACES) * 2 * M_PI;
			float x = cosf (angle) ;
			float z = sinf (angle) ;

			glNormal3f (x, 0.0, z);
			glVertex3f (x*width_parameter, 0.0, z*width_parameter);
			glVertex3f (x*width_parameter, params[0], z*width_parameter);
		}

		glEnd();
		glTranslatef(0,params[0],0);

/*		static GLUquadricObj *quadratic=gluNewQuadric();			// Create A Pointer To The Quadric Object ( NEW )
		if(params[1] > 0)
		{
			gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals ( NEW )
			gluSphere(quadratic,width_parameter*params[1],12,12);		// Draw A Sphere
		}*/
    }

    virtual void initGraphics()
    {
		glShadeModel(GL_SMOOTH);                    // shading mathod: GL_SMOOTH or GL_FLAT

		// enable /disable features
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		//glEnable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);

		// track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);

		// set up light colors (ambient, diffuse, specular)
		GLfloat lightKa[] = {.2f, .2f, .2f, 1.0f};  // ambient light
		GLfloat lightKd[] = {.7f, .7f, .7f, 1.0f};  // diffuse light
		GLfloat lightKs[] = {1, 1, 1, 1};           // specular light
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

		float lightPos[4] = {0, 20, -20.0, 0};
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		glEnable(GL_LIGHT0);
    }

    virtual void saveToMesh(const float* params) const
    {

    }

    virtual void voxel(const float* params) const
    {

    }

    virtual const char* symbolName() const
    { return "Stem"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_STEM_SYMBOL_HPP_
