#include "sceneObject.hpp"

#include <iostream>

SceneObject::SceneObject(const Eigen::Vector3d& position, const Eigen::Vector3d& up, const Eigen::Vector3d& forward) :
         m_position(position) 
{
    // Ensure that the forward vector is not zero
    if (forward.isZero()) {
        throw std::invalid_argument("Forward vector cannot be zero.");
    }
    // Ensure that the up vector is not zero
    if (up.isZero()) {
        throw std::invalid_argument("Up vector cannot be zero.");
    }

    // Ensure that the up vector is not parallel to the forward vector
    double up_forw_dp = up.dot(forward);
    if (up_forw_dp == 1.0 || up_forw_dp == -1.0) {
        throw std::invalid_argument("Up vector cannot be parallel to the forward vector.");
    }

    // Normalize the forward vector
    m_forward = forward.normalized();

    // Compute right as cross product of forward and up
    m_right = m_forward.cross(up).normalized();

    // Re-orthogonalize up in case up and forward were not perfectly orthogonal
    m_up = m_right.cross(m_forward).normalized();
}

void SceneObject::rotate(const Eigen::Vector3d& axis, double angle) {
    Eigen::AngleAxisd rotation(angle, axis.normalized());
    m_forward = rotation * m_forward;
    m_up = rotation * m_up;
    m_right = rotation * m_right;
}

/// @brief Convert a rotation vector to a rotation matrix
/// @param rotation_vector The rotation vector (3-dim vector in radians)
/// @return The rotation matrix corresponding to the rotation vector
Eigen::Matrix3d rotationVectorToMatrix(const Eigen::Vector3d& rotation_vector) {
    // Extract the angle (magnitude of the vector)
    double angle = rotation_vector.norm();

    // Calculate the normalized axis of rotation
    Eigen::Vector3d axis = rotation_vector.normalized();

    // Create an AngleAxis object
    Eigen::AngleAxisd angleAxis(angle, axis);

    // Convert to a rotation matrix
    return angleAxis.toRotationMatrix();
}

void SceneObject::rotate(Eigen::Vector3d& rotationVector) {
    Eigen::Matrix3d rotationMatrix = rotationVectorToMatrix(rotationVector);

    m_right = rotationMatrix * m_right;
    m_forward = rotationMatrix * m_forward;
    m_up = rotationMatrix * m_up;
}