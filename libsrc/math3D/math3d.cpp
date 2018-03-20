
#include "math3d.h"

Vector3f Vector3f::rotate(const Quaternion& rotation) const
{
    Quaternion conjugate_q = rotation.conjugate();
    Quaternion w = rotation * (*this) * conjugate_q;

    Vector3f ret(w.get_x(), w.get_y(), w.get_z());

    return ret;
}


