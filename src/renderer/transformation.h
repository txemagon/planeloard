#ifndef __TRANSFORMATION_H__
#define __TRANSFORMATION_H__

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Transformation
{
    public:
        Transformation(const glm::vec3 &pos=glm::vec3(), const glm::vec3 &rot=glm::vec3(), const glm::vec3 &scale=glm::vec3(1.0f, 1.0f, 1.0f)):
           m_pos(pos) , m_rot(rot), m_scale(scale) { }


        inline glm::mat4 get_model() const
        {
            glm::mat4 posMatrix = glm::translate(m_pos);
            glm::mat4 rotXMatrix = glm::rotate(m_rot.x, glm::vec3(1, 0, 0));
            glm::mat4 rotYMatrix = glm::rotate(m_rot.y, glm::vec3(0, 1, 0));
            glm::mat4 rotZMatrix = glm::rotate(m_rot.z, glm::vec3(0, 0, 1));
            glm::mat4 scaleMatrix = glm::scale(m_scale);

            glm::mat4 rotMatrix = rotXMatrix * rotYMatrix * rotZMatrix;

            return posMatrix * rotMatrix * scaleMatrix;
        }

        inline glm::vec3 &get_pos() { return m_pos; }
        inline glm::vec3 &get_rot() { return m_rot; }
        inline glm::vec3 &get_scale() {return m_scale; }

        inline void set_pos(const glm::vec3 pos) {m_pos = pos; }
        inline void set_rot(const glm::vec3 rot) {m_rot = rot; }
        inline void set_scale(const glm::vec3 scale) {m_scale = scale; }

    protected:
    private:
        Transformation(const Transformation &transformation) {}
        void operator=(const Transformation &transformation) {}

        glm::vec3 m_pos;
        glm::vec3 m_rot;
        glm::vec3 m_scale;
};

#endif
