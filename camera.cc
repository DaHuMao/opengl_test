#include "camera.h"

void Camera::PorcessKeyboard(CameraMovement direction,float delta_time) {
    float velocity = movement_speed_ * delta_time;
    if (CameraMovement::kForWard == direction) {
        camera_position_ += camera_front_ * velocity;
    } else if (CameraMovement::kBackWard == direction) {
        camera_position_ -= camera_front_ * velocity;
    } else if (CameraMovement::kLeft == direction) {
        camera_position_ -= camera_right_ * velocity; 
    } else if (CameraMovement::kRight == direction) {
        camera_position_ += camera_right_ * velocity;
    }
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrain_pitch) {
    xoffset *= mouse_sensitivety_;
    yoffset *= mouse_sensitivety_;
    yaw_ += xoffset;
    pitch_ += yoffset;
    if (constrain_pitch) {
        if (pitch_ > 89.0f) {
            pitch_ = 89.0f;
        }
        if (pitch_ < -89.0f) {
            pitch_ = -89.0f;
        }
    }
    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {

}

void Camera::UpdateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    camera_front_ = glm::normalize(front);

    camera_right_ = glm::normalize(glm::cross(camera_front_, word_up_));
    camera_up_ = glm::normalize(glm::cross(camera_right_, camera_front_));
}
