#include "Texture.h"

void sgl_Texture_Destroy(sgl_Texture* texture) {
    texture->vtable->Destroy(texture);
}

void sgl_Texture_Bind(sgl_Texture* texture, uint32 unit) {
    texture->vtable->Bind(texture, unit);
}