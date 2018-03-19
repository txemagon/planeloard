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

MeshData::MeshData(const IndexedModel &model) :
    ReferenceCounter(),
    m_draw_count(model.get_indices().get_size())
{
    if ( !model.is_valid( ))
    {
        std::cout << "Error: Invalid mesh! Must have same number of positions, "
            "tex_coords, normals, and tangents! "
            << "(Maybe you forgot to finalize() your IndexedModel?)" << std::endl;
        assert( 0 != 0 );
    }

    glGenVertexArrays( 1, &m_vertex_array_object );
    glBindVertexArray( m_vertex_array_object );

    glGenBuffers( NUM_BUFFERS, m_vertex_array_objects);

    glBindBuffer( GL_ARRAY_BUFFER, m_vertex_array_buffers[POSITION_VB] );
    glBufferData( GL_ARRAY_BUFFER, model.get_positions().size() * sizeof(model.get_positions()[0]), &model.get_positions[0] );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer( GL_ARRAY_BUFFER, m_vertex_array_buffers[TEXCOORD_VB] );
    glBufferData( GL_ARRAY_BUFFER, model.get_tex_coords().size() * sizeof(model.get_tex_coords()[0]), &model.get_tex_coords[0] );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer( GL_ARRAY_BUFFER, m_vertex_array_buffers[NORMAL_VB] );
    glBufferData( GL_ARRAY_BUFFER, model.get_normals().size() * sizeof(model.get_normals()[0]), &model.get_normals[0] );
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer( GL_ARRAY_BUFFER, m_vertex_array_buffers[TANGENT_VB] );
    glBufferData( GL_ARRAY_BUFFER, model.get_tangents().size() * sizeof(model.get_tangents()[0]), &model.get_tangents[0] );
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_vertex_array_buffers[INDEX_VB] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, model.get_indices().size() * sizeof(model.get_indices()[0]), &model.get_indices[0] );

}

MeshData::~MeshData()
{
    glDeleteBuffers(NUM_BUFFERS, m_vertex_array_buffers);
    glDeleteVertexArrays(1, &m_vertex_array_objects);
}

void MesData::draw() const
{
    glBindVertexArray(m_vertex_array_object);

#if PROFILING_DISABLE_MESH_DRAWING == 0
    glDrawElements(GL_TRIANGLES, m_draw_count, GL_UNSIGNED_INT, 0);
#endif
}

Mesh::Mesh(const std::string &mesh_name, const IndexedModel &model) :
    m_filename(mesh_name)
{
    std::map<std::string, MeshData *>::const Iterator it = s_resource_map.find(filename);
    if (it != s_resourceMap.end())
    {
        std::cout << "Error adding mesh " << mesh_name << ": Mesh already exists by the same name!" << std::endl;
        assert( 0 != 0 );
    }
    else
    {
        m_mesh_data = new MeshData(model);
        s_resource_map.insert(std::pair<std::string, MeshData *>(mesh_name, mesh_data));
    }

}

Mesh::Mesh(const std::string &mesh_name) :
    m_filename(mesh_name),
    m_mesh_data(0)
{
    std::map<std::string, MeshData *>::const Iterator it = s_resource_map.find(filename);
    if (it != s_resourceMap.end())
    {
        m_mesh_data = it->second;
        m_mesh_data -> add_reference();
    }
    else
    {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(("./res/models" + filename).c_str(),
                aiProcess_Triangulate |
                aiProcess_GenSmoothNormals |
                aiProcess_FlipUVs |
                aiProcess_CalcTangentSpace);
        if (!scene)
        {
            std::cout << "Mesh load failed!: " << filename <<std::endl;
            assert (0 == 0);
        }

        const aiMesh *model = scene->mMeshes[0];

        std::vector<Vector3f> positions;
        std::vector<Vector2f> tex_coords;
        std::vector<Vector3f> normals;
        std::vector<Vector3f> tangents;
        std::vector<unsigned> indices;

        const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
        for (unsigned i=0; i<model->mNumVertices; i++)
        {
            const aiVector3D pos = model->mVertices[i];
            const aiVector3D normal = model-> mNormals[i];
            const aiVector3D tex_coord = model->HasTextureCoords(0) ? model->mTextureCoords[0][i] : aiZeroVector;
            const aiVector3D tangent = model->mTangents[i];

            positions.push_back(Vector3f(pos.x, pos.y, pos.z));
            tex_coords.push_back(Vector3f(tex_coord.x, tex_coord.y, tex_coord.z));
            normals.push_back(Vector3f(normal.x, normal.y, normal.z));
            tangents.push_back(Vector3f(tangent.x, tangent.y, tangent.z));
        }

        for (unsigned i=0; i<model->mNumFaces; i++)
        {
            const aiFace &face = model->mFaces[i];
            assert(face.numIndices == 3);
            indices.push_back(face.mIndices[0]);
            indices.push_back(face.mIndices[1]);
            indices.push_back(face.mIndices[2]);
        }

        m_mesh_data = new MeshData(IndexedModel(indices, positions, tex_coords, normals, tangents));
        s_resource_map.insert(std::pair<std::string, MeshData *>(filename, m_mesh_data));

    }

}

Mesh::Mesh(const Mesh &mesh) :
    m_filename(mesh.m_filename),
    m_mesh_data(mesh.m_mesh_data)
{
  m_mesh_data -> add_reference ();
}

Mesh::~Mesh()
{
    if (m_mesh_data && m_mesh_data -> RemoveReference())
    {
        if (m_filename.length() > 0)
            s_resource_map.erase(m_filename);
        delete(m_mesh_data);
    }

}

Mesh::draw() const
{
    m_mesh_data -> draw();
}
