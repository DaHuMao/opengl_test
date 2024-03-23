#ifndef CAMERA_H_
#define CAMERA_H_

#include "third_party/glm/glm.hpp"
#include "third_party/glm/gtc/matrix_transform.hpp"

enum class CameraMovement {
    kForWard,
    kBackWard,
    kLeft,
    kRight
};


class Camera {
public:
    Camera() = default;
    inline void SetPosition(const glm::vec3& position) {
        camera_position_ = position;
    }

    inline void SetFront(const glm::vec3& front) {
        camera_front_ = front;
    }

    inline void SetWordUp(const glm::vec3& word_up) {
        word_up_ =  word_up;
    }

    inline glm::mat4 GetViewMatrix() const {
        return glm::lookAt(camera_position_,  camera_position_ + camera_front_,
                camera_up_);
    }

    inline void SetMouseSensitivety(float mouse_sensitivety) {
        mouse_sensitivety_ = mouse_sensitivety;
    }

    inline void SetMoveMentSpeed(float movement_speed) {
        movement_speed_ = movement_speed;
    }

    void PorcessKeyboard(CameraMovement direction, float delta_time);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrain_pitch = true);
    void ProcessMouseScroll(float yoffset);
private:
    void UpdateCameraVectors();

    glm::vec3 camera_position_ =  glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 camera_front_ = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 camera_up_ =  glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 camera_right_ =  glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 word_up_ =  glm::vec3(0.0f, 1.0f, 0.0f);
    float yaw_ = -90.0f;
    float pitch_ = 0.0f;
    float movement_speed_ = 2.5f;
    float mouse_sensitivety_ = 0.1f;
};

#endif // CAMERA_H_
