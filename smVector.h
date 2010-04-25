//-------------------------------------------------------------------
//-------------------------------------------------------------------
//
// Simple Spring Mass System
// -- vector lib
//
// Primary Author: James F. O'Brien (obrienj@cc.gatech.edu)
//
// (C) Copyright James F. O'Brien, 1995
// (C) Copyright Georgia Institute of Technology, 1995
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//
// RCS Revision History
//
// $Log: smVector.H,v $
// Revision 1.1  2004/09/08 00:01:46  adamb
// initial sources
//
// Revision 1.1.1.1  2004/07/08 08:33:33  adamb
// imported sources
//
// Revision 1.1.1.1  2003/11/05 23:56:30  adamb
// Imported sources
//
// Revision 1.1.1.1  2003/10/27 21:18:23  goktekin
// no message
//
// Revision 1.1.1.1  2003/03/17 10:03:53  adamb
// Initial Revision
//
// Revision 3.0  1999/03/11  22:29:14  obrienj
// Move from O32 ABI to n32
//
// Revision 2.7  1998/02/19  20:55:48  obrienj
// Fix declaration of cycle axis.
//
// Revision 2.6  1998/02/04  22:13:01  obrienj
// Cleaned up and made it easier to find stuff.
// index operators now take unsigned int rather than int
// if DEBUG defined,  do lots of range checking
// minComp() and maxComp() are now just min() and max()
// cycleAxis() is now a member of SmVector3
// Added class SmVector2
//
// Revision 2.5  1997/10/03  01:04:15  obrienj
// Added operator*(double,SmVector3)
//
// Revision 2.4  1997/09/11  20:14:43  obrienj
// Vector normalize check for div by zero.
//
// Revision 2.3  1997/05/30  19:58:33  obrienj
// Added sum and l1 and l2 norms
//
// Revision 2.2  1997/01/17  08:30:23  obrienj
// Added operator != .
//
// Revision 2.1  1996/10/04  04:17:57  obrienj
// Modified to compile under IRIX 6.2 ( mips2 ).
//
// Revision 1.1  1996/09/10  01:44:10  obrienj
// Initial revision
//
//
//-------------------------------------------------------------------
//-------------------------------------------------------------------

#ifndef SmVECTOR_is_defined
#define SmVECTOR_is_defined

//-------------------------------------------------------------------
//-------------------------------------------------------------------
// Classes from this header:

class SmVector2;
class SmVector3;

#include <cmath>
#include <cstdlib>
#include <iostream>

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class SmVector3 {
  // Class to store a three dimensional vector.
  // Expected uses include point in R3, RGB color, etc.
public:

  //----------------------------------------------
  // Constructors

  inline SmVector3();
  inline SmVector3(double d);
  inline SmVector3(double d0,double d1,double d2);

  inline SmVector3(const SmVector3 &da);
  inline SmVector3(const double    *da);  
  // da should point to a double[3] that will be copied


  //----------------------------------------------
  // Index operators

  inline double &operator[](unsigned int i)      ;
  inline double  operator[](unsigned int i) const;

  inline double &operator()(unsigned int i)      ;
  inline double  operator()(unsigned int i) const;


  //----------------------------------------------
  // Assignment and set

  inline SmVector3 &set(double d);
  inline SmVector3 &set(double d0, double d1, double d2);

  inline SmVector3 &set(const SmVector3 &da);
  inline SmVector3 &set(const double    *da);  
  // da should point to a double[3] that will be copied

  inline SmVector3 &operator=(double d);
  inline SmVector3 &operator=(const SmVector3 &da);
  inline SmVector3 &operator=(const double    *da);  
  

  //----------------------------------------------
  // Comparison operators

  inline int operator==(const SmVector3 &da) const;
  inline int operator!=(const SmVector3 &da) const;

  inline int operator==(double d) const;
  inline int operator!=(double d) const;


  //----------------------------------------------
  // In place arithmetic

  inline SmVector3 &operator+=(double d);
  inline SmVector3 &operator-=(double d);
  inline SmVector3 &operator*=(double d);
  inline SmVector3 &operator/=(double d);

  inline SmVector3 &operator+=(const SmVector3 &da);
  inline SmVector3 &operator-=(const SmVector3 &da);
  inline SmVector3 &operator*=(const SmVector3 &da);
  inline SmVector3 &operator/=(const SmVector3 &da);
  // Componentwise operations

  inline SmVector3 &maxSet(const SmVector3 &da);
  inline SmVector3 &minSet(const SmVector3 &da);
  // Sets data[i] = max(data[i],da[i]) or min


  //----------------------------------------------
  // Static methods

  inline static unsigned int cycleAxis(unsigned int axis, int direction);

  //----------------------------------------------
  // Define Components

  enum Index { X = 0 , Y = 1 , Z = 2 , 
	       U = 0 , V = 1 , W = 2 ,
	       R = 0 , G = 1 , B = 2 };

public:
  //----------------------------------------------
  // Public data members

  double data[3];

  //----------------------------------------------
};


//-------------------------------------------------------------------
//-------------------------------------------------------------------
// Operators for class SmVector3

inline SmVector3 operator-(const SmVector3 &a);

inline SmVector3 operator+(const SmVector3 &a,const SmVector3 &b);
inline SmVector3 operator-(const SmVector3 &a,const SmVector3 &b);
inline SmVector3 operator*(const SmVector3 &a,const SmVector3 &b);
inline SmVector3 operator/(const SmVector3 &a,const SmVector3 &b);

inline SmVector3 operator+(const SmVector3 &a, double b);
inline SmVector3 operator-(const SmVector3 &a, double b);
inline SmVector3 operator*(const SmVector3 &a, double b);
inline SmVector3 operator/(const SmVector3 &a, double b);

inline SmVector3 operator+(double a, const SmVector3 &b);
inline SmVector3 operator-(double a, const SmVector3 &b);
inline SmVector3 operator*(double a, const SmVector3 &b);
inline SmVector3 operator/(double a, const SmVector3 &b);

std::istream &operator>>(std::istream &strm,      SmVector3 &v);
std::ostream &operator<<(std::ostream &strm,const SmVector3 &v);

//-------------------------------------------------------------------
// Norm type functions for SmVector3

inline double   l1Norm(const SmVector3 &a);
inline double   l2Norm(const SmVector3 &a);
inline double lInfNorm(const SmVector3 &a);
// Computes the l1, l2 or lInfinity norm of a

inline double    mag(const SmVector3 &a);
inline double sqrMag(const SmVector3 &a);
// mag is the l2Norm or magnitude of the vector
// sqrMag is mag^2, which is faster to compute

inline void normalize(SmVector3 &a);
// SETS a = a/mag(a)


//-------------------------------------------------------------------
// Other functions for SmVector3

inline unsigned int dominantAxis(const SmVector3 &v);
inline unsigned int subinantAxis(const SmVector3 &v);
inline unsigned int midinantAxis(const SmVector3 &v);
// Returns the index of the component with the largest,
// smallest or middle value.  Note: subinantAxis and
// midinantAxis are nore really words, I made them up.
// If multiple comonents have the same value, then the
// results are not unique.

inline double      dot(const SmVector3 &a,const SmVector3 &b);
inline SmVector3 cross(const SmVector3 &a,const SmVector3 &b);
// Compute the dot and cros product of a and b.

inline double box(const SmVector3 &a,const SmVector3 &b,const SmVector3 &c);
// Compute the box (aka tripple) product of a, b, and d.

inline SmVector3 abs(const SmVector3 &a);
// returns a vector with r[i] = abs(a[i])

inline double sum(const SmVector3 &a);
// return a[0]+a[1]+a[2]

inline double max(const SmVector3 &a);
inline double min(const SmVector3 &a);
// Returns the max or min component of a.

inline SmVector3 max(const SmVector3 &a,const SmVector3 &b);
inline SmVector3 min(const SmVector3 &a,const SmVector3 &b);
// Computes a NEW vector by taking the max component in the
// x,y,z direction from a and b.  ie: r[i] = max(a[i],b[i])
// Note: signed values are used.


//-------------------------------------------------------------------
//-------------------------------------------------------------------


class SmVector2 {
  // Class to store a two dimensional vector.
  // Expected uses include point in R2, etc.
public:

  //----------------------------------------------
  // Constructors

  inline SmVector2();
  inline SmVector2(double d);
  inline SmVector2(double d0,double d1);

  inline SmVector2(const SmVector2 &da);
  inline SmVector2(const double    *da);  
  // da should point to a double[2] that will be copied


  //----------------------------------------------
  // Index operators

  inline double &operator[](unsigned int i)      ;
  inline double  operator[](unsigned int i) const;

  inline double &operator()(unsigned int i)      ;
  inline double  operator()(unsigned int i) const;


  //----------------------------------------------
  // Assignment and set

  inline SmVector2 &set(double d);
  inline SmVector2 &set(double d0, double d1);

  inline SmVector2 &set(const SmVector2 &da);
  inline SmVector2 &set(const double    *da);  
  // da should point to a double[3] that will be copied

  inline SmVector2 &operator=(double d);
  inline SmVector2 &operator=(const SmVector2 &da);
  inline SmVector2 &operator=(const double    *da);  
  
  //----------------------------------------------
  // Comparison operators

  inline int operator==(const SmVector2 &da) const;
  inline int operator!=(const SmVector2 &da) const;

  inline int operator==(double d) const;
  inline int operator!=(double d) const;


  //----------------------------------------------
  // In place arithmetic

  inline SmVector2 &operator+=(double d);
  inline SmVector2 &operator-=(double d);
  inline SmVector2 &operator*=(double d);
  inline SmVector2 &operator/=(double d);

  inline SmVector2 &operator+=(const SmVector2 &da);
  inline SmVector2 &operator-=(const SmVector2 &da);
  inline SmVector2 &operator*=(const SmVector2 &da);
  inline SmVector2 &operator/=(const SmVector2 &da);
  // Componentwise operations

  inline SmVector2 &maxSet(const SmVector2 &da);
  inline SmVector2 &minSet(const SmVector2 &da);
  // Sets data[i] = max(data[i],da[i]) or min


  //----------------------------------------------
  // Static methods

  inline static unsigned int cycleAxis(unsigned int axis, int direction);

  //----------------------------------------------
  // Define Components

  enum Index { X = 0 , Y = 1 ,
	       U = 0 , V = 1 };

public:
  //----------------------------------------------
  // Public data members

  double data[2];

  //----------------------------------------------
};


//-------------------------------------------------------------------
//-------------------------------------------------------------------
// Operators for class SmVector2

inline SmVector2 operator-(const SmVector2 &a);

inline SmVector2 operator+(const SmVector2 &a, const SmVector2 &b);
inline SmVector2 operator-(const SmVector2 &a, const SmVector2 &b);
inline SmVector2 operator*(const SmVector2 &a, const SmVector2 &b);
inline SmVector2 operator/(const SmVector2 &a, const SmVector2 &b);

inline SmVector2 operator+(const SmVector2 &a, double b);
inline SmVector2 operator-(const SmVector2 &a, double b);
inline SmVector2 operator*(const SmVector2 &a, double b);
inline SmVector2 operator/(const SmVector2 &a, double b);

inline SmVector2 operator+(double a, const SmVector2 &b);
inline SmVector2 operator-(double a, const SmVector2 &b);
inline SmVector2 operator*(double a, const SmVector2 &b);
inline SmVector2 operator/(double a, const SmVector2 &b);

std::istream &operator>>(std::istream &strm,      SmVector2 &v);
std::ostream &operator<<(std::ostream &strm,const SmVector2 &v);

//-------------------------------------------------------------------
// Norm type functions for SmVector2

inline double   l1Norm(const SmVector2 &a);
inline double   l2Norm(const SmVector2 &a);
inline double lInfNorm(const SmVector2 &a);
// Computes the l1, l2 or lInfinity norm of a

inline double    mag(const SmVector2 &a);
inline double sqrMag(const SmVector2 &a);
// mag is the l2Norm or magnitude of the vector
// sqrMag is mag^2, which is faster to compute

inline void normalize(SmVector2 &a);
// SETS a = a/mag(a)


//-------------------------------------------------------------------
// Other functions for SmVector2

inline unsigned int dominantAxis(const SmVector2 &v);
inline unsigned int subinantAxis(const SmVector2 &v);
// Returns the index of the component with the largest,
// smallest value.  Note: subinantAxis and is not really
// a word, I made it up.
// If multiple comonents have the same value, then the
// results are not unique.

inline double   dot(const SmVector2 &a,const SmVector2 &b);
inline double cross(const SmVector2 &a,const SmVector2 &b);
// Compute the dot and cros product of a and b.

inline SmVector2 abs(const SmVector2 &a);
// returns a vector with r[i] = abs(a[i])

inline double sum(const SmVector2 &a);
// return a[0]+a[1]

inline double max(const SmVector2 &a);
inline double min(const SmVector2 &a);
// Returns the max or min component of a.

inline SmVector2 max(const SmVector2 &a,const SmVector2 &b);
inline SmVector2 min(const SmVector2 &a,const SmVector2 &b);
// Computes a NEW vector by taking the max component in the
// x,y,z direction from a and b.  ie: r[i] = max(a[i],b[i])
// Note: signed values are used.




//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
// Inline implementation below:::

#ifdef DEBUG
#define DB_CHECK( C ) { if ( ! (C) ) { abort(); } }
#else
#define DB_CHECK( C ) { }
#endif

//-------------------------------------------------------------------
//-------------------------------------------------------------------

// Inline implementation of SmVector3

inline double &SmVector3::operator[](unsigned int i) {
  DB_CHECK(i<3);
  return data[i];
}
inline double &SmVector3::operator()(unsigned int i) {
  DB_CHECK(i<3);
  return data[i];
}

inline double  SmVector3::operator[](unsigned int i) const {
  DB_CHECK(i<3);
  return data[i];
}
  
inline double  SmVector3::operator()(unsigned int i) const {
  DB_CHECK(i<3);
  return data[i];
}

//-------------------------------------------------------------------

inline SmVector3::SmVector3() {
  data[0] = data[1] = data[2] = 0.0;
}
inline SmVector3::SmVector3(double d) {
  data[0] = data[1] = data[2] = d;
}

inline SmVector3::SmVector3(double d0,double d1,double d2) {
  data[0] = d0;
  data[1] = d1;
  data[2] = d2;
}

inline SmVector3::SmVector3(const SmVector3 &da) {
  data[0] = da[0];
  data[1] = da[1];
  data[2] = da[2];
}

inline SmVector3::SmVector3(const double *da) {
  data[0] = da[0];
  data[1] = da[1];
  data[2] = da[2];
}
  
//-------------------------------------------------------------------

inline SmVector3 &SmVector3::set(double d) {
  data[0] = d;
  data[1] = d;
  data[2] = d;
  return (*this);
}
  
inline SmVector3 &SmVector3::set(double d0, double d1, double d2) {
  data[0] = d0;
  data[1] = d1;
  data[2] = d2;
  return (*this);
}

inline SmVector3 &SmVector3::set(const SmVector3 &da) {
  data[0] = da[0];
  data[1] = da[1];
  data[2] = da[2];
  return (*this);
}
  
inline SmVector3 &SmVector3::set(const double *da) {
  data[0] = da[0];
  data[1] = da[1];
  data[2] = da[2];
  return (*this);
}
  
//-------------------------------------------------------------------

inline SmVector3 &SmVector3::operator=(double d) {
  return set(d);
}
 
inline SmVector3 &SmVector3::operator=(const SmVector3 &da) {
  return set(da);
}

inline SmVector3 &SmVector3::operator=(const double *da) {
  return set(da);
}
  
//-------------------------------------------------------------------

inline int SmVector3::operator==(const SmVector3 &da) const {
  return ((data[0] == da[0]) &&
	  (data[1] == da[1]) &&
	  (data[2] == da[2]));
}

inline int SmVector3::operator!=(const SmVector3 &da) const {
  return ((data[0] != da[0]) ||
	  (data[1] != da[1]) ||
	  (data[2] != da[2]));
}
  
inline int SmVector3::operator==(double d) const {
  return ((data[0] == d) &&
	  (data[1] == d) &&
	  (data[2] == d));
}

inline int SmVector3::operator!=(double d) const {
  return ((data[0] != d) ||
	  (data[1] != d) ||
	  (data[2] != d));
}

//-------------------------------------------------------------------

inline SmVector3 &SmVector3::operator+=(double d) {
  data[0] += d;
  data[1] += d;
  data[2] += d;
  return (*this);
}
    
inline SmVector3 &SmVector3::operator-=(double d) {
  data[0] -= d;
  data[1] -= d;
  data[2] -= d;
  return (*this);
}

inline SmVector3 &SmVector3::operator*=(double d) {
  data[0] *= d;
  data[1] *= d;
  data[2] *= d;
  return (*this);
}

inline SmVector3 &SmVector3::operator/=(double d) {
  data[0] /= d;
  data[1] /= d;
  data[2] /= d;
  return (*this);
}

//-------------------------------------------------------------------

inline SmVector3 &SmVector3::operator+=(const SmVector3 &da) {
  data[0] += da[0];
  data[1] += da[1];
  data[2] += da[2];
  return (*this);
}

inline SmVector3 &SmVector3::operator-=(const SmVector3 &da) {
  data[0] -= da[0];
  data[1] -= da[1];
  data[2] -= da[2];
  return (*this);
}

inline SmVector3 &SmVector3::operator*=(const SmVector3 &da) {
  data[0] *= da[0];
  data[1] *= da[1];
  data[2] *= da[2];
  return (*this);
}

inline SmVector3 &SmVector3::operator/=(const SmVector3 &da) {
  data[0] /= da[0];
  data[1] /= da[1];
  data[2] /= da[2];
  return (*this);
}

//-------------------------------------------------------------------

inline SmVector3 &SmVector3::maxSet(const SmVector3 &da) {
  if (da[0] > data[0]) data[0] = da[0];
  if (da[1] > data[1]) data[1] = da[1];
  if (da[2] > data[2]) data[2] = da[2];
  return (*this);
}

inline SmVector3 &SmVector3::minSet(const SmVector3 &da) {
  if (da[0] < data[0]) data[0] = da[0];
  if (da[1] < data[1]) data[1] = da[1];
  if (da[2] < data[2]) data[2] = da[2];
  return (*this);
}
  
//-------------------------------------------------------------------

inline unsigned int SmVector3::cycleAxis(unsigned int axis, int direction) {
  switch (axis+direction) {
  case 0: case 3: case 6: return 0;
  case 1: case 4: case 7: return 1;
  case 2: case 5: case 8: return 2;
  default: return (axis+direction)%3;
  }
}

//-------------------------------------------------------------------

inline SmVector3 operator-(const SmVector3 &a) {
  return SmVector3(-a[0],-a[1],-a[2]);
}

//-------------------------------------------------------------------

inline SmVector3 operator+(const SmVector3 &a,const SmVector3 &b) {
  return SmVector3(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}

inline SmVector3 operator-(const SmVector3 &a,const SmVector3 &b) {
  return SmVector3(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}

inline SmVector3 operator*(const SmVector3 &a,const SmVector3 &b){
  return SmVector3(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
}

inline SmVector3 operator/(const SmVector3 &a,const SmVector3 &b){
  return SmVector3(a[0] / b[0], a[1] / b[1], a[2] / b[2]);
}

//-------------------------------------------------------------------

inline SmVector3 operator+(const SmVector3 &a,double b){
  return SmVector3(a[0] + b, a[1] + b, a[2] + b);
}

inline SmVector3 operator-(const SmVector3 &a,double b){
  return SmVector3(a[0] - b, a[1] - b, a[2] - b);
}

inline SmVector3 operator*(const SmVector3 &a,double b){
  return SmVector3(a[0] * b, a[1] * b, a[2] * b);
}

inline SmVector3 operator/(const SmVector3 &a,double b){
  return SmVector3(a[0] / b, a[1] / b, a[2] / b);
}

//-------------------------------------------------------------------

inline SmVector3 operator+(double a,const SmVector3 &b){
  return SmVector3(a + b[0], a + b[1], a + b[2]);
}

inline SmVector3 operator-(double a,const SmVector3 &b){
  return SmVector3(a - b[0], a - b[1], a - b[2]);
}

inline SmVector3 operator*(double a,const SmVector3 &b){
  return SmVector3(a * b[0], a * b[1], a * b[2]);
}

inline SmVector3 operator/(double a,const SmVector3 &b){
  return SmVector3(a / b[0], a / b[1], a / b[2]);
}

//-------------------------------------------------------------------

inline double l1Norm(const SmVector3 &a) {
  return (((a[0]>0)?a[0]:-a[0])+
	  ((a[1]>0)?a[1]:-a[1])+
	  ((a[2]>0)?a[2]:-a[2]));
}
  
inline double l2Norm(const SmVector3 &a) {
  return sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
}
  
inline double lInfNorm(const SmVector3 &a) {
  return max(abs(a));
}

inline double mag(const SmVector3 &a) {
  return sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
}

inline double sqrMag(const SmVector3 &a) {
  return (a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
}

inline void normalize(SmVector3 &a) {
  register double m = mag(a);
  if (m != 0) a /= m;
}

//-------------------------------------------------------------------

inline unsigned int dominantAxis(const SmVector3 &v) {
  register double x,y,z;
  if (v[0]>0) x = v[0]; else x = -v[0];
  if (v[1]>0) y = v[1]; else y = -v[1];
  if (v[2]>0) z = v[2]; else z = -v[2];
  return ( x > y ) ? (( x > z ) ? 0 : 2) : (( y > z ) ? 1 : 2 );
}

inline unsigned int subinantAxis(const SmVector3 &v) {
  register double x,y,z;
  if (v[0]>0) x = v[0]; else x = -v[0];
  if (v[1]>0) y = v[1]; else y = -v[1];
  if (v[2]>0) z = v[2]; else z = -v[2];
  return ( x < y ) ? (( x < z ) ? 0 : 2) : (( y < z ) ? 1 : 2 );
}

inline unsigned int midinantAxis(const SmVector3 &v) {
  register double x,y,z;
  if (v[0]>0) x = v[0]; else x = -v[0];
  if (v[1]>0) y = v[1]; else y = -v[1];
  if (v[2]>0) z = v[2]; else z = -v[2];
  register unsigned int d = ( x > y ) ? (( x > z ) ? 0 : 2) : (( y > z ) ? 1 : 2 );
  register unsigned int s = ( x < y ) ? (( x < z ) ? 0 : 2) : (( y < z ) ? 1 : 2 );
  register unsigned int m;
  if (d==0) {
    if (s!= 1) m = 1; else m = 2;
  }else if (d==1) {
    if (s!= 0) m = 0; else m = 2;
  }else if (d==2) {
    if (s!= 0) m = 0; else m = 1;
  }
  return m;
}

//-------------------------------------------------------------------

inline double dot(const SmVector3 &a,const SmVector3 &b) {
  return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
}
  
inline SmVector3 cross(const SmVector3 &a,const SmVector3 &b) {
  return SmVector3(a[1] * b[2] - b[1] * a[2],
		   a[2] * b[0] - b[2] * a[0],
		   a[0] * b[1] - b[0] * a[1]);
}

//-------------------------------------------------------------------

inline double box(const SmVector3 &a,const SmVector3 &b,const SmVector3 &c) {
  return dot(cross(a,b),c);
}

//-------------------------------------------------------------------

inline SmVector3 abs(const SmVector3 &a) {
  return  SmVector3(((a[0]>0)?a[0]:-a[0]),
		    ((a[1]>0)?a[1]:-a[1]),
		    ((a[2]>0)?a[2]:-a[2]));
} 

inline double sum(const SmVector3 &a) {
  return a[0]+a[1]+a[2];
}

//-------------------------------------------------------------------

inline double max(const SmVector3 &a) {
  return ((a[0]>a[1])?((a[0]>a[2])?a[0]:a[2]):(a[1]>a[2])?a[1]:a[2]);
}	  

inline double min(const SmVector3 &a) {
  return ((a[0]<a[1])?((a[0]<a[2])?a[0]:a[2]):(a[1]<a[2])?a[1]:a[2]);
}	  

//-------------------------------------------------------------------

inline SmVector3 max(const SmVector3 &a,const SmVector3 &b) {
  return SmVector3((a[0]>b[0])?a[0]:b[0],
		   (a[1]>b[1])?a[1]:b[1],
		   (a[2]>b[2])?a[2]:b[2]);
}
 
inline SmVector3 min(const SmVector3 &a,const SmVector3 &b) {
  return SmVector3((a[0]<b[0])?a[0]:b[0],
		   (a[1]<b[1])?a[1]:b[1],
		   (a[2]<b[2])?a[2]:b[2]);
}


//-------------------------------------------------------------------
//-------------------------------------------------------------------

// Inline implementation of SmVector2

inline double &SmVector2::operator[](unsigned int i) {
  DB_CHECK(i<2);
  return data[i];
}
inline double &SmVector2::operator()(unsigned int i) {
  DB_CHECK(i<2);
  return data[i];
}

inline double  SmVector2::operator[](unsigned int i) const {
  DB_CHECK(i<2);
  return data[i];
}
  
inline double  SmVector2::operator()(unsigned int i) const {
  DB_CHECK(i<2);
  return data[i];
}

//-------------------------------------------------------------------

inline SmVector2::SmVector2() {
  data[0] = data[1] = 0.0;
}
inline SmVector2::SmVector2(double d) {
  data[0] = data[1] = d;
}

inline SmVector2::SmVector2(double d0,double d1) {
  data[0] = d0;
  data[1] = d1;
}

inline SmVector2::SmVector2(const SmVector2 &da) {
  data[0] = da[0];
  data[1] = da[1];
}

inline SmVector2::SmVector2(const double *da) {
  data[0] = da[0];
  data[1] = da[1];
}
  
//-------------------------------------------------------------------

inline SmVector2 &SmVector2::set(double d) {
  data[0] = d;
  data[1] = d;
  return (*this);
}
  
inline SmVector2 &SmVector2::set(double d0, double d1) {
  data[0] = d0;
  data[1] = d1;
  return (*this);
}

inline SmVector2 &SmVector2::set(const SmVector2 &da) {
  data[0] = da[0];
  data[1] = da[1];
  return (*this);
}
  
inline SmVector2 &SmVector2::set(const double *da) {
  data[0] = da[0];
  data[1] = da[1];
  return (*this);
}
  
//-------------------------------------------------------------------

inline SmVector2 &SmVector2::operator=(double d) {
  return set(d);
}
 
inline SmVector2 &SmVector2::operator=(const SmVector2 &da) {
  return set(da);
}

inline SmVector2 &SmVector2::operator=(const double *da) {
  return set(da);
}
  
//-------------------------------------------------------------------

inline int SmVector2::operator==(const SmVector2 &da) const {
  return ((data[0] == da[0]) &&
	  (data[1] == da[1]));
}

inline int SmVector2::operator!=(const SmVector2 &da) const {
  return ((data[0] != da[0]) ||
	  (data[1] != da[1]));
}
  
inline int SmVector2::operator==(double d) const {
  return ((data[0] == d) &&
	  (data[1] == d));
}

inline int SmVector2::operator!=(double d) const {
  return ((data[0] != d) ||
	  (data[1] != d));
}

//-------------------------------------------------------------------

inline SmVector2 &SmVector2::operator+=(double d) {
  data[0] += d;
  data[1] += d;
  return (*this);
}
    
inline SmVector2 &SmVector2::operator-=(double d) {
  data[0] -= d;
  data[1] -= d;
  return (*this);
}

inline SmVector2 &SmVector2::operator*=(double d) {
  data[0] *= d;
  data[1] *= d;
  return (*this);
}

inline SmVector2 &SmVector2::operator/=(double d) {
  data[0] /= d;
  data[1] /= d;
  return (*this);
}

//-------------------------------------------------------------------

inline SmVector2 &SmVector2::operator+=(const SmVector2 &da) {
  data[0] += da[0];
  data[1] += da[1];
  return (*this);
}

inline SmVector2 &SmVector2::operator-=(const SmVector2 &da) {
  data[0] -= da[0];
  data[1] -= da[1];
  return (*this);
}

inline SmVector2 &SmVector2::operator*=(const SmVector2 &da) {
  data[0] *= da[0];
  data[1] *= da[1];
  return (*this);
}

inline SmVector2 &SmVector2::operator/=(const SmVector2 &da) {
  data[0] /= da[0];
  data[1] /= da[1];
  return (*this);
}

//-------------------------------------------------------------------

inline SmVector2 &SmVector2::maxSet(const SmVector2 &da) {
  if (da[0] > data[0]) data[0] = da[0];
  if (da[1] > data[1]) data[1] = da[1];
  return (*this);
}

inline SmVector2 &SmVector2::minSet(const SmVector2 &da) {
  if (da[0] < data[0]) data[0] = da[0];
  if (da[1] < data[1]) data[1] = da[1];
  return (*this);
}
  
//-------------------------------------------------------------------

inline unsigned int SmVector2::cycleAxis(unsigned int axis, int direction) {
  switch (axis+direction) {
  case 0: case 2: case 4: return 0;
  case 1: case 3: case 5: return 1;
  default: return (axis+direction)%2;
  }
}

//-------------------------------------------------------------------

inline SmVector2 operator-(const SmVector2 &a) {
  return SmVector2(-a[0],-a[1]);
}

//-------------------------------------------------------------------

inline SmVector2 operator+(const SmVector2 &a,const SmVector2 &b) {
  return SmVector2(a[0] + b[0], a[1] + b[1]);
}

inline SmVector2 operator-(const SmVector2 &a,const SmVector2 &b) {
  return SmVector2(a[0] - b[0], a[1] - b[1]);
}

inline SmVector2 operator*(const SmVector2 &a,const SmVector2 &b){
  return SmVector2(a[0] * b[0], a[1] * b[1]);
}

inline SmVector2 operator/(const SmVector2 &a,const SmVector2 &b){
  return SmVector2(a[0] / b[0], a[1] / b[1]);
}

//-------------------------------------------------------------------

inline SmVector2 operator+(const SmVector2 &a,double b){
  return SmVector2(a[0] + b, a[1] + b);
}

inline SmVector2 operator-(const SmVector2 &a,double b){
  return SmVector2(a[0] - b, a[1] - b);
}

inline SmVector2 operator*(const SmVector2 &a,double b){
  return SmVector2(a[0] * b, a[1] * b);
}

inline SmVector2 operator/(const SmVector2 &a,double b){
  return SmVector2(a[0] / b, a[1] / b);
}

//-------------------------------------------------------------------

inline SmVector2 operator+(double a,const SmVector2 &b){
  return SmVector2(a + b[0], a + b[1]);
}

inline SmVector2 operator-(double a,const SmVector2 &b){
  return SmVector2(a - b[0], a - b[1]);
}

inline SmVector2 operator*(double a,const SmVector2 &b){
  return SmVector2(a * b[0], a * b[1]);
}

inline SmVector2 operator/(double a,const SmVector2 &b){
  return SmVector2(a / b[0], a / b[1]);
}

//-------------------------------------------------------------------

inline double l1Norm(const SmVector2 &a) {
  return (((a[0]>0)?a[0]:-a[0])+
	  ((a[1]>0)?a[1]:-a[1]));
}
  
inline double l2Norm(const SmVector2 &a) {
  return sqrt(a[0] * a[0] + a[1] * a[1]);
}
  
inline double lInfNorm(const SmVector2 &a) {
  return max(abs(a));
}

inline double mag(const SmVector2 &a) {
  return sqrt(a[0] * a[0] + a[1] * a[1]);
}

inline double sqrMag(const SmVector2 &a) {
  return (a[0] * a[0] + a[1] * a[1]);
}

inline void normalize(SmVector2 &a) {
  register double m = mag(a);
  if (m != 0) a /= m;
}

//-------------------------------------------------------------------

inline unsigned int dominantAxis(const SmVector2 &v) {
  register double x,y;
  if (v[0]>0) x = v[0]; else x = -v[0];
  if (v[1]>0) y = v[1]; else y = -v[1];
  return ( x > y ) ? 0 : 1;
}

inline unsigned int subinantAxis(const SmVector2 &v) {
  register double x,y;
  if (v[0]>0) x = v[0]; else x = -v[0];
  if (v[1]>0) y = v[1]; else y = -v[1];
  return ( x < y ) ? 0 : 1;
}

//-------------------------------------------------------------------

inline double dot(const SmVector2 &a,const SmVector2 &b) {
  return (a[0] * b[0] + a[1] * b[1]);
}
  
inline double cross(const SmVector2 &a,const SmVector2 &b) {
  return (a[0] * b[1] - b[0] * a[1]);
}

//-------------------------------------------------------------------

inline SmVector2 abs(const SmVector2 &a) {
  return  SmVector2(((a[0]>0)?a[0]:-a[0]),
		    ((a[1]>0)?a[1]:-a[1]));
} 

inline double sum(const SmVector2 &a) {
  return a[0]+a[1];
}

//-------------------------------------------------------------------

inline double max(const SmVector2 &a) {
  return ((a[0]>a[1])? a[0] : a[1]);
}	  

inline double min(const SmVector2 &a) {
  return ((a[0]<a[1])? a[0] : a[1]);
}	  

//-------------------------------------------------------------------

inline SmVector2 max(const SmVector2 &a,const SmVector2 &b) {
  return SmVector2((a[0]>b[0])?a[0]:b[0],
		   (a[1]>b[1])?a[1]:b[1]);
}
 
inline SmVector2 min(const SmVector2 &a,const SmVector2 &b) {
  return SmVector2((a[0]<b[0])?a[0]:b[0],
		   (a[1]<b[1])?a[1]:b[1]);
}



//-------------------------------------------------------------------
//-------------------------------------------------------------------

#endif

