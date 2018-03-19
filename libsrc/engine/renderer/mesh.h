#ifndef __MESH_H__
#define __MESH_H__

#define ROOTLIB_DIR ../../
#define CONC(x,y) ROOTLIB_DIR ## y
#define LIB(x) # CONC(x)

#include "math3d/math3d.h"
#include "profiler/reference_counter.h"

#include <string>
#include <vector>
#include <map>
#include <GL/glew.h>

class IndexedModel
{
    public:
        IndexedModel();
        IndexedModel(const std::vector<unsigned> indices,
                const std::vector<Vector3f> &positions,
                const std::vector<Vector2f> &tex_coords,
                const std::vector<Vector3f> &normals = std::vector<Vector3f>(),
                const std::vector<Vector3f> &tangents = std::vector<Vector3f>()) :
            m_indices(indices),
            m_positions(positions),
            m_tex_coords(tex_coords),
            m_normals(normals),
            m_tangents(tangents) {}

        bool is_valid() const;
        void calc_normals();
        void calc_tangents();

        IndexedModel finalize();

        void add_vertex(const Vector3f &vert);
        inline void add_vertex(float x, float y, float z) { add_vertex (Vector3f(x,y,z)); }

        void add_tex_coord(const Vector2f &tex_coord);
        inline void add_tex_coord(float x, float y) { add_tex_coord (Vector2f(x,y)); }

        void add_normal(const Vector3f &normal);
        inline void add_normal(float x, float y, float z) { add_normal (Vector3f(x,y,z)); }

        void add_tangent(const Vector3f &tangent);
        inline void add_tangent(float x, float y, float z) { add_tangent (Vector3f(x,y,z)); }

        void add_face(unsigned vert_index0, unsigned vert_index1, unsigned vert_index2);

        inline const std::vector<unsigned> &get_indices()    const { return m_indices;    }
        inline const std::vector<Vector3f> &get_positions()  const { return m_positions;  }
        inline const std::vector<Vector2f> &get_tex_coords() const { return m_tex_coords; }
        inline const std::vector<Vector3f> &get_normals()    const { return m_normals;    }
        inline const std::vector<Vector3f> &get_tangents()   const { return m_tangents;   }

    private:
        std::vector<unsigned> m_indices;
        std::vector<Vector3f> m_positions;
        std::vector<Vector2f> m_tex_coords;
        std::vector<Vector3f> m_normals;
        std::vector<Vector3f> m_tangents;
};


class MeshData : public ReferenceCounter
{
    public:
        MeshData(const IndexedModel &model);
        virtual ~MeshData();

        void draw() const;

    private:
        MeshData(const MeshData &other) {}
        void operator=(const MeshData &other) {}

        enum
        {
            POSITION_VB,
            TEXCOORD_VB,
            NORMAL_VB,
            TANGENT_VB,

            INDEX_VB,

            NUM_BUFFERS
        };

        GLuint m_vertex_array_object;
        GLuint m_vertex_array_buffer[NUM_BUFFERS];
        int m_draw_count;
};

class Mesh {
    public:
        Mesh(const std::string &filename = "cube.obj");
        Mesh(const std::string &mesh_name, const IndexedModel &model);
        virtual ~Mesh();

        void draw() const;

    protected:
    private:
        static std::map<std::string, MeshData *> s_resource_map;

        std::string m_filename;
        MeshData *m_mesh_data;

        void operator=(Mesh &mesh) {}
};
#endif // MESH_H
