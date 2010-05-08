#ifndef OBJ_HPP_INCLUDED
#define OBJ_HPP_INCLUDED

#include "obj_config.hpp"
#include "obj.hpp"
#include "obj_parser.hpp"

#endif // OBJ_HPP_INCLUDED


#ifndef LSCULPT_OBJ_H
#define LSCULPT_OBJ_H

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>
#include <iostream>

#include <tr1/functional>

#include "LSculpt_classes.h"

class obj_loader
{
public:
  obj_loader() {}
  bool convert(std::istream& istream, std::vector<Triangle>& mesh, ArgumentSet& args);
  ArgumentSet* LSculpt_args;
  std::vector<Triangle>* LSculpt_mesh;
private:
  void info_cb(const std::string& filename, std::size_t line_number, const std::string& message) {}
  void warning_cb(const std::string& filename, std::size_t line_number, const std::string& message) {}
  void error_cb(const std::string& filename, std::size_t line_number, const std::string& message);
  void vertex_cb(obj::float_type x, obj::float_type y, obj::float_type z);
  void face_cb_v(obj::index_type v1, obj::index_type v2, obj::index_type v3);
  void face_cb_v_vt(const obj::index_2_tuple_type& v1_vt1, const obj::index_2_tuple_type& v2_vt2, const obj::index_2_tuple_type& v3_vt3);
  void face_cb_v_vn(const obj::index_2_tuple_type& v1_vn1, const obj::index_2_tuple_type& v2_vn2, const obj::index_2_tuple_type& v3_vn3);
  void face_cb_v_vt_vn(const obj::index_3_tuple_type& v1_vt1_vn1, const obj::index_3_tuple_type& v2_vt2_vn2, const obj::index_3_tuple_type& v3_vt3_vn3);
  std::ostream* ostream_;
  std::vector<std::tr1::tuple<obj::float_type, obj::float_type, obj::float_type> > vertices_;  
};

void obj_loader::error_cb(const std::string& filename, std::size_t line_number, const std::string& message)
{
  std::cerr << filename << ":" << line_number << ": " << "error: " << message << std::endl;
}

void obj_loader::vertex_cb(obj::float_type x, obj::float_type y, obj::float_type z)
{
  switch (this->LSculpt_args->OPTS_UP) {
    case UP_Z:
      vertices_.push_back(std::tr1::tuple<obj::float_type, obj::float_type, obj::float_type >(x, z, -y));
      break;
    default:
      vertices_.push_back(std::tr1::tuple<obj::float_type, obj::float_type, obj::float_type >(x, y, z));
      break;
  }
}

void obj_loader::face_cb_v(obj::index_type v1, obj::index_type v2, obj::index_type v3)
{
  // add new triangle to the mesh
  this->LSculpt_mesh->push_back(Triangle());

  // set the vertices of the triangle
  this->LSculpt_mesh->back().v[0] = SmVector3(std::tr1::get<0>(vertices_[v1 - 1]),std::tr1::get<1>(vertices_[v1 - 1]),std::tr1::get<2>(vertices_[v1 - 1]));
  this->LSculpt_mesh->back().v[1] = SmVector3(std::tr1::get<0>(vertices_[v2 - 1]),std::tr1::get<1>(vertices_[v2 - 1]),std::tr1::get<2>(vertices_[v2 - 1]));
  this->LSculpt_mesh->back().v[2] = SmVector3(std::tr1::get<0>(vertices_[v3 - 1]),std::tr1::get<1>(vertices_[v3 - 1]),std::tr1::get<2>(vertices_[v3 - 1]));
}

void obj_loader::face_cb_v_vt(const obj::index_2_tuple_type& v1_vt1, const obj::index_2_tuple_type& v2_vt2, const obj::index_2_tuple_type& v3_vt3)
{
  this->face_cb_v(std::tr1::get<0>(v1_vt1),std::tr1::get<0>(v2_vt2),std::tr1::get<0>(v3_vt3));
}

void obj_loader::face_cb_v_vn(const obj::index_2_tuple_type& v1_vn1, const obj::index_2_tuple_type& v2_vn2, const obj::index_2_tuple_type& v3_vn3)
{
  this->face_cb_v(std::tr1::get<0>(v1_vn1),std::tr1::get<0>(v2_vn2),std::tr1::get<0>(v3_vn3));
}

void obj_loader::face_cb_v_vt_vn(const obj::index_3_tuple_type& v1_vt1_vn1, const obj::index_3_tuple_type& v2_vt2_vn2, const obj::index_3_tuple_type& v3_vt3_vn3)
{
  this->face_cb_v(std::tr1::get<0>(v1_vt1_vn1),std::tr1::get<0>(v2_vt2_vn2),std::tr1::get<0>(v3_vt3_vn3));
}


bool obj_loader::convert(std::istream& istream, std::vector<Triangle>& mesh, ArgumentSet& args)
{
  using namespace std::tr1::placeholders;

  // turn on triangulate faces, and translate relative indices
  obj::obj_parser::flags_type obj_parser_flags = 0;
  obj_parser_flags |= obj::obj_parser::triangulate_faces;
  obj_parser_flags |= obj::obj_parser::translate_negative_indices;

  obj::obj_parser obj_parser(obj_parser_flags);

  std::string ifilename;

  // set callback functions
  obj_parser.info_callback(std::tr1::bind(&obj_loader::info_cb, this, ifilename, _1, _2));
  obj_parser.warning_callback(std::tr1::bind(&obj_loader::warning_cb, this, ifilename, _1, _2));
  obj_parser.error_callback(std::tr1::bind(&obj_loader::error_cb, this, ifilename, _1, _2));

  obj_parser.geometric_vertex_callback(std::tr1::bind(&obj_loader::vertex_cb, this, _1, _2, _3));
  obj_parser.face_callbacks(
    std::tr1::bind(&obj_loader::face_cb_v, this, _1, _2, _3),
    std::tr1::bind(&obj_loader::face_cb_v_vt, this, _1, _2, _3),
    std::tr1::bind(&obj_loader::face_cb_v_vn, this, _1, _2, _3),
    std::tr1::bind(&obj_loader::face_cb_v_vt_vn, this, _1, _2, _3),
    NULL, NULL, NULL, NULL, // quadrilateral face cbs (4)
    NULL, NULL, NULL, NULL, // polygon face cbs (12)
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL
  );

  this->LSculpt_mesh = &mesh;
  this->LSculpt_args = &args;

  return obj_parser.parse(istream);
}

#endif // LSCULPT_OBJ_H
