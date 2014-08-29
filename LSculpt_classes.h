/*	LSculpt: Studs-out LEGO® Sculpture

	Copyright (C) 2010 Bram Lambrecht <bram@bldesign.org>

	http://lego.bldesign.org/LSculpt/

	This file (LSculpt_classes.h) is part of LSculpt.

	LSculpt is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	LSculpt is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see http://www.gnu.org/licenses/  */

// 
// Class definitions for PLY/STL to LDR oriented voxelization
//

#ifndef LSCULPT_CLASSES_H
#define LSCULPT_CLASSES_H

#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <QFileInfo>
#include <QString>
#include "smVector.h"	// 2D and 3D vector library
#include "rply.h"		// PLY input library

using namespace std;

#define PI 3.1415926535897932384626433832795

// shortcuts for SpCube flags
#define ORIENT_0         0x0001  // 0000 0000 0000 0001
#define ORIENT_1         0x0002  // 0000 0000 0000 0010
#define ORIENT_2         0x0004  // 0000 0000 0000 0100
#define ORIENT_NEG       0x0008  // 0000 0000 0000 1000
#define ORIENT_CLEAR     0xFFF8  // 1111 1111 1111 1000
#define NEIGHBOR_0_PLUS  0x0010  // 0000 0000 0001 0000
#define NEIGHBOR_0_NEG   0x0020  // 0000 0000 0010 0000
#define NEIGHBOR_1_PLUS  0x0040  // 0000 0000 0100 0000
#define NEIGHBOR_1_NEG   0x0080  // 0000 0000 1000 0000
#define NEIGHBOR_2_PLUS  0x0100  // 0000 0001 0000 0000
#define NEIGHBOR_2_NEG   0x0200  // 0000 0010 0000 0000
#define NEIGHBOR_CLEAR   0xFC0F  // 1111 1100 0000 1111
#define THIN_0           0x0400  // 0000 0100 0000 0000
#define THIN_1           0x0800  // 0000 1000 0000 0000
#define THIN_2           0x1000  // 0001 0000 0000 0000
#define THIN_CLEAR       0xE3FF  // 1110 0011 0000 0000

// Lego geometry specifics
// dimensions are in LDU (1 LDU = 0.4 MM)
#define SPCUBE_WIDTH  40
#define SPCUBE_PAD     4
#define VOXEL_WIDTH   20
#define VOXEL_HEIGHT   8

// Filenames for commandline option -a
#define VOXEL_PN_0    "3024.DAT"
#define VOXEL_PN_1    "3070B.DAT"
#define VOXEL_PN_2    "3024FAST.DAT"
#define VOXEL_PN_3    "3070FAST.DAT"


#define GRID_PN       "GRIDBOX.DAT"
#define LDR_PREC      3

//
// ===== Argument definitions ======
//

#define FORMAT_PLY 1
#define FORMAT_STL 2
#define FORMAT_OBJ 3
#define MESSAGE_NONE 0
#define MESSAGE_ERR  1
#define MESSAGE_ALL  2
#define COLOR_LAY    1
#define COLOR_DIR    2
#define COLOR_OFF    0
#define UP_Y 0
#define UP_Z 1

// Scale conversion factors
#define UNIT_LDU_MM  0.40005
#define UNIT_LDU_CM  0.040005
#define UNIT_LDU_M   0.00040005
#define UNIT_LDU_IN  0.01575
#define UNIT_LDU_FT  0.0013125
#define UNIT_LDU_ST  0.05

// Maximum iterations for optimization
#define OPTIM_MAX    100000

struct ArgumentSet {
	bool          OPTS_CENTER;  // center mesh?
	unsigned char OPTS_FORMAT;  // input file format
	unsigned char OPTS_MESSAGE; // verbosity
	bool          OPTS_STUDSUP; // initialize studs-up instead of studs-out
	bool          OPTS_NOFILL;  // just convert the surface, don't try to fill it
	unsigned char OPTS_BASE;    // bias studs-up at the bottom
	unsigned char OPTS_UP;      // up vector of input mesh
	SmVector3     OPTS_OFFSET;  // amount to offset input mesh
	double        OPTS_FIT;     // size to fit input mesh to
	double        OPTS_SCALE;   // scale factor for mesh
	double        OPTS_ROT;     // rotation of mesh
	double        OPTS_ROT_SIN; // sin of said rotation
	double        OPTS_ROT_COS; // cos of said rotation
	int           OPTS_MAXITER; // maximum optimization iterations
	unsigned char OPTS_PART;    // part to use for output
	unsigned char OPTS_COLOR;   // color scheme for output
	bool          OPTS_GRID;    // show the grid (space partitioning cubes) in output
	bool          OPTS_MESH;    // show the input mesh in output
	bool          OPTS_PLATES;  // show the result in output

	int           OPTS_COLOR_PLATES;
	int           OPTS_COLOR_MESH;
	int           OPTS_COLOR_GRID;
	
	// LDraw colors
	int           OPTS_COLOR_NONE;
	int           OPTS_COLOR_0;
	int           OPTS_COLOR_1;
	int           OPTS_COLOR_2;
	int           OPTS_COLOR_3;
	int           OPTS_COLOR_4;
	int           OPTS_COLOR_5;

	// Energy functional weights for cube optimization
	double OP_ORN;  // Weight of orientation with respect to cube's average normal
	double OP_DIR;  // Weight of direction with respect to cube's average normal
	double OP_NBR;  // Weight of orientation/direction of neighbors
	double OP_THN;  // Importance of whether a neighbor is thin or not
	double OP_NCT;  // Importance of the number of neighboring cubes a neighbor has
	double OP_SOR;  // Importance of neighbors with the same orientation but different direction
	double OP_BAK;  // Importance of neighbor directly behind or in front of cube
	double OP_BKO;  // Importance of neighbor directly behind or in front of cube with same orientation

    double       meshsize;
};

static const ArgumentSet defaultArgs = {
	true,        // OPTS_CENTER
	0,           // OPTS_FORMAT
	MESSAGE_ERR, // OPTS_MESSAGE
	false,       // OPTS_STUDSUP
	false,       // OPTS_NOFILL
	0,           // OPTS_BASE
	UP_Y,        // OPTS_UP
	SmVector3(0.0, 0.0, 0.0), // OPTS_OFFSET
	0.0,         // OPTS_FIT
	1.0,         // OPTS_SCALE
	0.0,         // OPTS_ROT
	0.0,         // OPTS_ROT_SIN
	1.0,         // OPTS_ROT_COS
	OPTIM_MAX,   // OPTS_MAXITER
	0,           // OPTS_PART
	COLOR_OFF,   // OPTS_COLOR
	false,       // OPTS_GRID
	false,       // OPTS_MESH
	true,        // OPTS_PLATES

	16,          // OPTS_COLOR_PLATES
	9,           // OPTS_COLOR_MESH
  47,          // OPTS_COLOR_GRID
	
	16,          // OPTS_COLOR_NONE
	7,           // OPTS_COLOR_0 - Gray
	4,           // OPTS_COLOR_1 - Red
	14,          // OPTS_COLOR_2 - Yellow
	15,          // OPTS_COLOR_3 - White
	1,           // OPTS_COLOR_4 - Blue
	2,           // OPTS_COLOR_5 - Green

	0.25,        // OP_ORN
	0.25,        // OP_DIR
	0.50,        // OP_NBR
	0.00,        // OP_THN
	0.00,        // OP_NCT
	0.00,        // OP_SOR
	0.00,        // OP_BAK
	0.00,        // OP_BKO

    0.00         // meshsize
};


// ====== Class Declarations =======

// custom types used by classes
#define VoxelList   unsigned int
#define SpCubeCoord char
#define SpCubeFlags unsigned short

class Triangle;		// Polygon object face class: 3 vertices, bounding box, and normal
class Hit;			// A triangle/ray intersection: distance and boolean inside/outside

class SpCubeKey;	// Location of a space partitioning cube 
class SpCube;		// A space partitioning cube: face list and various properties
class SpCubeEnergy; // Optimization data: location of a space partitioning cube,
                    // the amount its energy can be reduced, and the best orientation

// ====== Class Definitions =======
class Triangle {
public:
	SmVector3 v[3], // vertices
		an,			// area normal
		un,         // unit normal
		mn, mx;	// bounding box

	SmVector3 computenormal(bool);
	SmVector3 computenormal() { return computenormal(false); };
	void	  computebounds(SmVector3 &, SmVector3 &);
	double    area();
	SmVector3 unitnormal() { return (un != SmVector3()) ? un : computenormal(false); };
	SmVector3 areanormal() { return (an != SmVector3()) ? an : computenormal(true); };
	bool      intersect(SmVector3 &, unsigned char);
};

class SpCubeKey {
public:
	SpCubeCoord loc[3];

	inline SpCubeKey() {};
	inline SpCubeKey(SpCubeCoord,SpCubeCoord,SpCubeCoord);
	inline SpCubeKey(const SpCubeKey &);
	inline SpCubeKey &operator=(const SpCubeKey &);
	inline bool operator<(const SpCubeKey &) const;
	inline bool operator==(const SpCubeKey &) const;
	inline bool operator!=(const SpCubeKey &) const;
	inline SpCubeCoord &operator[](unsigned char i)       { return loc[i]; };
	inline SpCubeCoord  operator[](unsigned char i) const { return loc[i]; };
};

class SpCube {
public:
	vector<Triangle *> tlist;  // list of triangles
	SmVector3 avgnormal;	   // average normal of triangles
	VoxelList voxels[3];       // list of Lego piece voxels in each direction
	SpCubeFlags flags;         // information about contents of the cube
	multiset<SpCubeEnergy>::iterator energy; // pointer to the energy of the cube
	                                         // in the optimization queue	

	// Mark the passed orientation (0-2) as "thin"
	inline unsigned char markthin(unsigned char);

	// Orient the cube in the passed orientation (0-2)
	inline unsigned char orientset(unsigned char);

	// Clear the orientation flags
	inline void orientclear() { flags &= ORIENT_CLEAR; };

	// Return the current orientation (0-2)
	inline unsigned char orientget();

	// Change the direction to -orientation
	inline void orientneg() { flags |= ORIENT_NEG; };

	// Change the direction to +orientation
	inline void orientpos() { flags &= ~ORIENT_NEG; };

	// Mark neighbor in the passed direction as existing
	inline void neighboron(unsigned char, bool);

	// Count the number of neighbors marked to exist
	inline unsigned char neighborcount();

	// Remove all neighbor records
	inline void neighborclear() { flags &= NEIGHBOR_CLEAR; };

	// Check if the neighbor at +-(0-2) is marked
	inline bool isneighboron(unsigned char, bool);

	// Check if the neighbor (0-5) is marked
	inline bool isneighboron(unsigned char);

	// Check if the direction is -orientation
	inline bool isorientneg() { return flags & ORIENT_NEG; };

	// Check if the cube is thin in the passed orientation
    inline bool isthin(unsigned char);

	// Check if the cube is thin in the current orientation
	inline bool isthin() { return isthin(orientget()); };
};

class Hit {
public:
	double position;
	bool inside;
	inline bool operator<(const Hit &a) const { return (position < a.position) || (position == a.position && inside && !a.inside); };
	inline bool operator==(const Hit &a) const { return position == a.position; };
};

class SpCubeEnergy {
public:
	double reduce;			// the amount the energy can be reduced by
	                        // by switching the orientation of the cube
	                        // stored at location "cube" to the orientation
	                        // indicated by "flags"
	SpCubeKey cube;
	unsigned char flags;

	// Sort the cubes from highest to lowest energy
	inline bool operator<(const SpCubeEnergy &a) const { return reduce > a.reduce; };  // reverse sort

	// Set the best orientation
	inline unsigned char orientset(unsigned char);
	inline void orientclear() { flags &= ORIENT_CLEAR; };
	inline void orientneg() { flags |= ORIENT_NEG; };
	inline void orientpos() { flags &= ~ORIENT_NEG; };

	// Return the best orientation
	inline unsigned char orientget() const;
	inline bool isorientneg() const { return flags & ORIENT_NEG; };
};

//
// ====== Class Member Function Definitions =======
//

//
// Triangle member functions
//
inline SmVector3 Triangle::computenormal(bool area)
{
	an = cross(v[1] - v[0], v[2] - v[0]);
	un = an / mag(an);
	return area ? an : un;
}

inline double Triangle::area()
{
	if(an == SmVector3()) computenormal();
	return mag(an);
}

inline void Triangle::computebounds(SmVector3 &retmin, SmVector3 &retmax)
// Return the bounds via the pass by reference minimum and maximum positions
// Also, store the result in the class members mn, mx
{
	mn[0] = min(v[0][0],min(v[1][0],v[2][0]));
	mn[1] = min(v[0][1],min(v[1][1],v[2][1]));
	mn[2] = min(v[0][2],min(v[1][2],v[2][2]));
	mx[0] = max(v[0][0],max(v[1][0],v[2][0]));
	mx[1] = max(v[0][1],max(v[1][1],v[2][1]));
	mx[2] = max(v[0][2],max(v[1][2],v[2][2]));
	retmin = mn;
	retmax = mx;
}

inline double whichside(SmVector2 a, SmVector2 b, SmVector2 c)
// Test which side of line a-b point c is on (in the plane)
{
	return a[0]*(b[1] - c[1]) + b[0]*(c[1] - a[1]) + c[0]*(a[1] - b[1]);
}

inline bool Triangle::intersect(SmVector3 &pt, unsigned char dir)
// intersect a ray aligned with axis dir through the passed
// pt with the triangle.  If an intersection exists, return true
// and move pass by reference "pt" to the point of intersection.
{
	unsigned char j, k;
	double r[3];
	j = (dir+1)%3; k = (dir+2)%3;	
	
	// check pt vs bounding box in orthogonal directions:
	if (pt[j] > mn[j] && pt[j] <= mx[j] && pt[k] > mn[k] && pt[k] <= mx[k]) {
		// 2D projection of triangle
		SmVector2 v2d[3], p2d;
		v2d[0] = SmVector2(v[0][j],v[0][k]);
		v2d[1] = SmVector2(v[1][j],v[1][k]);
		v2d[2] = SmVector2(v[2][j],v[2][k]);
		p2d = SmVector2(pt[j], pt[k]);

		// side test of point with each edge of the triangle
		r[0] = whichside(v2d[0],v2d[1],p2d);
		r[1] = whichside(v2d[1],v2d[2],p2d);
		r[2] = whichside(v2d[2],v2d[0],p2d);

		if ((r[0] >= 0 && r[1] >=0 && r[2] >= 0) || (r[0] <= 0 && r[1] <=0 && r[2] <= 0)){
			// calculate actual intersection with plane of triangle
			pt[dir] += 1/un[dir] * dot(un, v[0] - pt);
			return true;
		}
	}
	return false;
}


//
// Space partitioning cube location member functions
//
inline SpCubeKey::SpCubeKey(const SpCubeKey &b)
// copy constructor
{	loc[0] = b.loc[0]; loc[1] = b.loc[1]; loc[2] = b.loc[2]; }

inline SpCubeKey::SpCubeKey(SpCubeCoord x, SpCubeCoord y, SpCubeCoord z)
// constructor from x, y, z location
{	loc[0] = x; loc[1] = y; loc[2] = z; }


inline SpCubeKey &SpCubeKey::operator=(const SpCubeKey &b)
// assignment
{	loc[0] = b.loc[0]; loc[1] = b.loc[1]; loc[2] = b.loc[2]; return (*this); }

inline bool SpCubeKey::operator<(const SpCubeKey &a) const
// sort by vertical direction (y) first, then x, then z
{
	return (
		((*this).loc[1] < a.loc[1]) ||
		(((*this).loc[1] <= a.loc[1]) && 
			(((*this).loc[0] < a.loc[0]) ||
			(((*this).loc[0] <= a.loc[0]) &&
				((*this).loc[2] < a.loc[2])
				)
			)
		)
	);
}

inline bool SpCubeKey::operator==(const SpCubeKey &a) const
{	return ((*this).loc[0] == a.loc[0]) && 
           ((*this).loc[1] == a.loc[1]) &&
		   ((*this).loc[2] == a.loc[2]);	}
inline bool SpCubeKey::operator!=(const SpCubeKey &a) const
{	return ((*this).loc[0] != a.loc[0]) || 
           ((*this).loc[1] != a.loc[1]) ||
		   
		   ((*this).loc[2] != a.loc[2]);	}

//
// Space partitioning cube member functions
//
inline unsigned char SpCube::markthin(unsigned char d)
{
	switch(d) {
	case 0:
		flags |= THIN_0;
		break;
	case 1:
		flags |= THIN_1;
		break;
	case 2:
		flags |= THIN_2;
		break;
	}
	return d;
}
inline void SpCube::neighboron(unsigned char d, bool plus)
{
	switch(d*2 + ((plus) ? 0 : 1)) {
		case 0:
			flags |= NEIGHBOR_0_PLUS;
			break;
		case 1:
			flags |= NEIGHBOR_0_NEG;
			break;
		case 2:
			flags |= NEIGHBOR_1_PLUS;
			break;
		case 3:
			flags |= NEIGHBOR_1_NEG;
			break;
		case 4:
			flags |= NEIGHBOR_2_PLUS;
			break;
		case 5:
			flags |= NEIGHBOR_2_NEG;
			break;
	}
}
inline bool SpCube::isneighboron(unsigned char d, bool plus)
{
	switch(d*2 + ((plus) ? 0 : 1)) {
		case 0:
			return flags & NEIGHBOR_0_PLUS;
		case 1:
			return flags & NEIGHBOR_0_NEG;
		case 2:
			return flags & NEIGHBOR_1_PLUS;
		case 3:
			return flags & NEIGHBOR_1_NEG;
		case 4:
			return flags & NEIGHBOR_2_PLUS;
		case 5:
			return flags & NEIGHBOR_2_NEG;
	}
	return false;
}
inline bool SpCube::isneighboron(unsigned char n)
{
	switch(n) {
		case 0:
			return flags & NEIGHBOR_0_PLUS;
		case 1:
			return flags & NEIGHBOR_0_NEG;
		case 2:
			return flags & NEIGHBOR_1_PLUS;
		case 3:
			return flags & NEIGHBOR_1_NEG;
		case 4:
			return flags & NEIGHBOR_2_PLUS;
		case 5:
			return flags & NEIGHBOR_2_NEG;
	}
	return false;
}
inline unsigned char SpCube::neighborcount()
{
	unsigned char c;
	SpCubeFlags n = flags & ~NEIGHBOR_CLEAR;
	for(c = 0; n; c++) n &= n - 1;
	return c;
}
inline unsigned char SpCube::orientset(unsigned char d)
{
	orientclear();
	switch(d) {
	case 0:
		flags |= ORIENT_0;
		break;
	case 1:
		flags |= ORIENT_1;
		break;
	case 2:
		flags |= ORIENT_2;
		break;
	}
	return d;
}
inline unsigned char SpCube::orientget()
{
	return (flags & ORIENT_0) ? 0 : ((flags & ORIENT_1) ? 1 : 2);
}
inline bool SpCube::isthin(unsigned char d)
{
	switch(d) {
	case 0:
		return flags & THIN_0;
	case 1:
		return flags & THIN_1;
	case 2:
		return flags & THIN_2;
	}
	return false;	
}
//
// Space partitioning cube optimization energy member functions
//
inline unsigned char SpCubeEnergy::orientset(unsigned char d)
{
	orientclear();
	switch(d) {
	case 0:
		flags |= ORIENT_0;
		break;
	case 1:
		flags |= ORIENT_1;
		break;
	case 2:
		flags |= ORIENT_2;
		break;
	}
	return d;
}
inline unsigned char SpCubeEnergy::orientget() const
{
	return (flags & ORIENT_0) ? 0 : ((flags & ORIENT_1) ? 1 : 2);
}

#endif // LSCULPT_CLASSES_H

