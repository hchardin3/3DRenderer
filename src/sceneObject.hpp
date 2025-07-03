#pragma once

#include <Eigen/Dense>

class SceneObject {
    private:
        /// @brief The up vector of the object in the global frame
        Eigen::Vector3d m_up;

        /// @brief The forward vector of the object in the global frame
        Eigen::Vector3d m_forward;

        /// @brief The right vector of the object in the global frame
        Eigen::Vector3d m_right;

        /// @brief The rotation matrix of the object in the global frame
        Eigen::Matrix3d m_rotationMatrix;

    protected:
        /// @brief The position of the object in the global frame
        Eigen::Vector3d m_position;

        /// @brief A method to get the up vector of the object in the global frame
        /// @return The up vector of the object in global frame
        const Eigen::Vector3d& getUp() const { return m_up; }

        /// @brief A method to get the forward vector of the object in the global frame
        /// @return The forward vector of the object in global frame
        const Eigen::Vector3d& getForward() const { return m_forward; }

        /// @brief A method to get the right vector of the object in the global frame
        /// @return The right vector of the object in global frame
        const Eigen::Vector3d& getRight() const { return m_right; }

        /// @brief A method to get the rotation matrix of the object in the global frame
        /// @return The rotation matrix of the object in global frame
        const Eigen::Matrix3d& getRotationMatrix() const { return m_rotationMatrix; }
    
    public:
        /// @brief A simple scene object that can be placed in the scene
        /// @param position The position of the object in the global frame
        /// @param up The up vector of the object in the global frame
        /// @param forward The forward vector of the object in the global frame
        SceneObject(const Eigen::Vector3d& position, const Eigen::Vector3d& up, const Eigen::Vector3d& forward);

        /// @brief A method to get the position of the object in the global frame
        /// @return The 3D position vector of the object in global frame
        Eigen::Vector3d getPosition() const {
            return m_position;
        };

        /// @brief A method to set the position of the object in the global frame
        /// @param position The new position of the object in the global frame
        void setPosition(const Eigen::Vector3d& position) {
            m_position = position;
        };

        /// @brief A method to translate the object along a displacement vector
        /// @param displacement The displacement vector
        /// @note The translation is applied to the position of the object.
        /// @note The up, forward, and right vectors are not changed by this method.
        void translate(const Eigen::Vector3d& displacement) {
            m_position += displacement;
        };

        /// @brief A method to rotate the object around a given axis
        /// @param axis The axis of rotation (must be a unit vector)
        /// @param angle The angle of rotation in radians
        /// @note The rotation is applied to the forward, up, and right vectors of the object.
        void rotate(const Eigen::Vector3d& axis, double angle);

        /// @brief A method to rotate the object around a given rotation vector
        /// @param rotationVector The rotation vector (angle and axis of rotation)
        /// @note The rotation is applied to the forward, up, and right vectors of the object.
        /// @note The rotation vector is expected to be in the form of (angle * axis)
        void rotate(Eigen::Vector3d& rotationVector);
};