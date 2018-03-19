#include "mesh.h"

#include "profiler/profiling.h"

#include <GL/glew.h>
#include <iostream>

#include <vector>
#include <cassert>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

std::map<std::string, MeshData *> Mesh::s_resource_map;

bool IndexedModel::is_valid(  ) const
{
    return m_positions.size()  == m_tex_coords.size()
        && m_tex_coords.size() == m_normals.size()
        && m_normals.size()    == m_tangents.size();
}

void IndexedModel::add_vertex( const Vector3f &vert ) {
    m_positions.push_back(vert);
}

void IndexedModel::add_tex_coord( const Vector2f &tex_coord) {
    m_positions.push_back (tex_coord);
}

void IndexedModel::add_normal( const Vector3f &normal )
{
    m_normals.push_back (normal);
}

void IndexedModel::add_tangent( const Vector3f &tangent)
{
    m_tangents.push_back (tangent);
}

IndexedModel IndexedModel::finalize()
{
    if (is_valid ())
        return *this;

    if (m_tex_coords.size() == 0)
        for (unsigned i = m_tex_coords.size(); i<m_positions.size(); i++)
            m_tex_coords.push_back(Vector2f(0., 0.));
    if (m_normals.size() == 0)
        calc_normals ();
    if (m_tangents.size() == 0)
        calc_tangents ();

    return *this;
}

void IndexedModel::add_face(unsigned vert_index0, unsigned vert_index1, unsigned vert_index2)
{
    m_indices.push_back (vert_index0);
    m_indices.push_back (vert_index1);
    m_indices.push_back (vert_index2);
}

void IndexedModel::calc_normals()
{
    m_normals.clear ();
    m_normals.reserve (m_positions.size());

    for(unsigned i=0; i<m_positions.size(); i++)
        m_normals.push_back(Vector3f(0., 0., 0.));

    for(unsigned i=0; i<m_indices.size(); i+=3)
    {
        int i0 = m_indices[i];
        int i1 = m_indices[i+1];
        int i2 = m_indices[i+2];

        Vector3f v1 = m_positions[i1] - m_positions[i0];
        Vector3f v2 = m_positions[i2] - m_positions[i0];

        Vector3f normal = v1.cross(v2).normalized();

        m_normals[i0] = m_normals[i0] + normal;
        m_normals[i1] = m_normals[i1] + normal;
        m_normals[i2] = m_normals[i2] + normal;
    }

    for(unsigned i=0; i<m_normals.size(); i++)
        m_normals[i] = m_normals[i].normalized();
}

void IndexedModel::calc_tangents()
{
    m_tangents.clear ();
    m_tangents.reserve (m_positions.size());

    for (unsigned i=0; i<m_positions.size(); i++)
        m_tangents.push_back(Vector3f(0., 0., 0.));

    for (unsigned i=0; i<m_indices.size(); i+=3)
    {
        int i0 = m_indices[i];
        int i1 = m_indices[i+1];
        int i2 = m_indices[i+2];

        Vector3f edge1 = m_positions[i1] - m_positions[i0];
        Vector3f edge2 = m_positions[i2] - m_positions[i0];

        float deltaU1 = m_tex_coords[i1].get_x() - m_tex_coords[i0].get_x();
        float deltaU2 = m_tex_coords[i2].get_x() - m_tex_coords[i0].get_x();
        float deltaV1 = m_tex_coords[i1].get_y() - m_tex_coords[i0].get_y();
        float deltaV2 = m_tex_coords[i2].get_y() - m_tex_coords[i0].get_y();

        float dividend = deltaU1 * deltaV2 - deltaU2 * deltaV1;
        float f = dividend == 0.0f ? 0.0f : 1.0f / dividend;

        Vector3f tangent = Vector3f(0, 0, 0);
        tangent.set_x(f * (deltaV2 * edge1.get_x() - deltaV1 * edge2.get_x() )) ;
        tangent.set_y(f * (deltaV2 * edge1.get_y() - deltaV1 * edge2.get_y() )) ;
        tangent.set_z(f * (deltaV2 * edge1.get_z() - deltaV1 * edge2.get_z() )) ;

        m_tangents[i0] += tangent;
        m_tangents[i1] += tangent;
        m_tangents[i2] += tangent;
     }

    for (unsigned i=0; i<m_tangents.size(); i++)
        m_tangents[i] = m_tangents[i].normalized();

}
