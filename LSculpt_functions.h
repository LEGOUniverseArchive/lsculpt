/*	LSculpt: Studs-out LEGO® Sculpture

	Copyright (C) 2010 Bram Lambrecht <bram@bldesign.org>

	http://lego.bldesign.org/LSculpt/

	This file (LSculpt_functions.h) is part of LSculpt.

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
// Function definitions for PLY/STL to LDR oriented voxelization
// Comments describing each function are in LSculpt_functions.h
//

#ifndef LSCULPT_FUNCTIONS_H
#define LSCULPT_FUNCTIONS_H

#include "LSculpt_classes.h"  // Class definitions and declarations

using namespace std;

//
// ===== Function Declarations =====
//

float now();			// Return the time since operation started
void strupper(char *);	// Uppercase a string

// Load command line options into global variables
void load_options(int argc, char*argv[], char* in, char* out);

// Set the file format argument based on the input file name's extension
void setFileFormat(ArgumentSet *localArgs, char *in);

// Set an output filename, if there is none
void setOutFile(ArgumentSet localArgs, char *in, char *out);

// Setting studs up base argument isn't a simple set, so wrap it
void setStudsUpBaseArg(ArgumentSet *localArgs, int value);

// Keep default argument set inside LSculpt engine, and access it with this
ArgumentSet getDefaultArgumentSet();

// Allow global LSculpt argument set to be set elsewhere
void setArgumentSet(ArgumentSet args);

// Print out the help file
void usage();

// Rotate a vector about y axis by the amount stored in OPTS_ROT
inline SmVector3 roty(SmVector3);

// Load triangles from a OBJ file using the libobj library
bool load_triangles_obj(char *f);

// Load triangles from a ply file using the rPly library
bool load_triangles_ply(char *f);

// callback function for vertices
int myply_vertex_cb(p_ply_argument argument);

// callback function for faces
int myply_face_cb(p_ply_argument argument);

// Load triangles from a binary STL file
bool load_triangles_stl(char *f);

// Load triangles from an ascii STL file
bool load_triangles_stla(char *f);

// Compute the normal vectors and bounding box for each face
// in the input mesh, and return the bounding box of the overall
// mesh in the pass by reference variables
bool mesh_bounds(SmVector3 &, SmVector3 &);

// Intersect the bounding box of each face with the space
// partitioning cube grid and update the triangle lists in
// intersected cubes.  Pad bounding boxes by SPCUBE_PAD to
// avoid holes in the subsequent voxelization
void partition_space();

// Find an average normal vector for the faces in each
// space partitioning cube
void compute_cube_normals();

// Initialize the voxelization in each cube using voxelize
// Orient each cube according to the maximum component of the
// average normal vector
void init_voxels();

// Voxelize the cube at the pointer and the passed location
// according to the orientation and faces in the cube by
// ray casting.  If no intersections are found for a ray, 
// call intersectneighbors
void voxelize(SpCubeKey, SpCube *);

// Test a triangle against a ray through the passed point in the
// passed direction.  If it hits, add the intersection to the
// list and return true
inline bool recordintersect(Triangle *, SmVector3 &, const unsigned char &, set<Hit> *);

// Recursively test neighboring cubes to SpCubeKey in the passed direction until
// an intersection is found or there are no more neighbors
bool intersectneighbors(SpCubeKey, SmVector3 &, const unsigned char, set<Hit> *, bool);

// Update each cube's flags with information about which of that
// cube's neighbors are still in existance after the voxelization
void identify_neighbors();

// return a pointer to the specified neighbor (0-5) of a location
inline SpCube *get_neighbor(SpCubeKey, unsigned char);

// Initialize the optimization queue by iterating through
// every cube and running update_energy
void initialize_energy();

// Compute the six possible energies for a cube given 
// a pointer to the cube, it's location, and pointers to its
// six neighbors (or null if a neighbor does not exist)
// If the cube can be reoriented to be improved, add the cube
// to the queue.
void update_energy(SpCube *cubeptr[7], SpCubeKey loc);

// Compute the energy of a cube in a given direction given
// pointers to it and its six neighbors
double compute_energy(SpCube *cubes[7], unsigned char, bool);

// Run the optimization routine by popping the worst cube,
// reorienting the cube, and updating its neighbors.
unsigned int optimize_voxels();

// Write the grid in LDraw format
// Returns true on success, false on error
bool ldraw_grid(ofstream &, char *);

// Write the triangle mesh in LDraw format
// Returns true on success, false on error
bool ldraw_mesh(ofstream &, char *);

// Write the plates in LDraw format
// Returns true on success, false on error
bool ldraw_plates(ofstream &, char *);

// Write the output file
// Returns true on success, false on error
bool save_ldraw(char *f);

// temp wrapper fore existing console's main() function
int main_wrapper(char *infile, char *outfile, void (*progress_cb)(const char *));

#endif // LSCULPT_FUNCTIONS_H
