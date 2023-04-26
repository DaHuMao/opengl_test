#ifndef TEXTURE_LOAD_H_
#define TEXTURE_LOAD_H_
#include <string>
class TextureLoad {
public:
    TextureLoad(const std::string& file_path);
    ~TextureLoad();
    void Bind(uint32_t slot = 0) const;
    void UnBind() const;
    inline int GetWidth() const { return texture_width_; }
    inline int GetHeight() const { return texture_height_; }
private:
    uint32_t texture_id_ = 0;
    int texture_width_ = 0;
    int texture_height_ = 0;
   // int texture_bpp_ = 0;
};

#endif // TEXTURE_H_
