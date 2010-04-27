// LSculpt_functions.h
// Bram Lambrecht <bram@bldesign.org>
// 2006.12.14
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

// Print out the help file
void usage();

// Rotate a vector about y axis by the amount stored in OPTS_ROT
inline SmVector3 roty(SmVector3);

// Load triangles from a ply file using the rPly library
bool load_triangles_ply(char *f);
	// callback function for vertices
	static int myply_vertex_cb(p_ply_argument argument);
	// callback function for faces
	static int myply_face_cb(p_ply_argument argument);

// Load triangles from a binary STL file
bool load_triangles_stl(char *f);

// Load triangles from an ascii STL file
bool load_triangles_stla(char *f);

// Compute the normal vectors and bounding box for each face
// in the input mesh, and return the bounding box of the overall
// mesh in the pass by reference variables
void mesh_bounds(SmVector3 &, SmVector3 &);

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

// Write the output file
void save_ldraw(char *f);

//
// ===== More definitions ======
//

#define FORMAT_PLY 1
#define FORMAT_STL 2
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

//
// ===== Global variables =====
//

// Triangle mesh input:  Rotation of the mesh is done during
// file input.  Scaling and translation is done on the fly
// by scaling and translating the space partitioning cubes
vector<Triangle>       inputmesh; 

// Pairing of each space partitioning cube with its location
// in space.  The map allows easy lookup of a cube based
// on its location in space.
map<SpCubeKey, SpCube> cubelist; 

// Queue of optimization energies
multiset<SpCubeEnergy> cubeenergy;

//
// ===== Command line options =====
//

unsigned char OPTS_FORMAT  = 0;            // input file format
unsigned char OPTS_MESSAGE = MESSAGE_ERR;  // verbosity
bool          OPTS_CENTER  = false;        // center mesh?
bool          OPTS_STUDSUP = false;        // initialize studs-up instead of studs-out
bool          OPTS_NOFILL  = false;        // just convert the surface, don't try to fill it
unsigned char OPTS_BASE    = 0;            // bias studs-up at the bottom
unsigned char OPTS_UP      = UP_Y;         // up vector of input mesh
SmVector3     OPTS_OFFSET;                 // amount to offset input mesh
double        OPTS_FIT     = 0.0;          // size to fit input mesh to
double        OPTS_SCALE   = 1.0;          // scale factor for mesh
double        OPTS_ROT     = 0.0;          // rotation of mesh
double        OPTS_ROT_SIN = 0.0;          // sin of said rotation
double        OPTS_ROT_COS = 1.0;          // cos of said rotation
int           OPTS_MAXITER = OPTIM_MAX;    // maximum optimization iterations
unsigned char OPTS_PART    = 0;            // part to use for output
unsigned char OPTS_COLOR   = COLOR_OFF;    // color scheme for output

// Energy functional weights for cube optimization
double OP_ORN =   0.25;  // Weight of orientation with respect to cube's average normal
double OP_DIR =   0.25;  // Weight of direction with respect to cube's average normal
double OP_NBR =   0.50;  // Weight of orientation/direction of neighbors
double OP_THN =   0.00;  // Importance of whether a neighbor is thin or not
double OP_NCT =   0.00;  // Importance of the number of neighboring cubes a neighbor has
double OP_SOR =   0.00;  // Importance of neighbors with the same orientation but different direction
double OP_BAK =   0.00;  // Importance of neighbor directly behind or in front of cube
double OP_BKO =   0.00;  // Importance of neighbor directly behind or in front of cube with same orientation

#endif // LSCULPT_FUNCTIONS_H
