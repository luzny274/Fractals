#ifndef BU_SKYBROT
#define BU_SKYBROT

#include "fractal.cpp"
#include "skyShader.cpp"

class SkyBrot : public buf::Fractal{
    private:
        ulm::SkyShaderToy shader;

        float scale = 1.f;
        ulm::Camera camera;
        float time = 0.f;
        int maxIter = 50;

        float speed = 2.f;
        float scaleSpeed = 10.f;

        float FOV = 50.f;

    public:

        SkyBrot(){
            camera.createPerspective(glm::radians(FOV), (float)ulm::Window::width / (float)ulm::Window::height, 0.1f, 100000.f);
            shader.code = ulm::String(R"====(
                in vec3 direction;

                uniform float aspectRatio;

                uniform float scale;
                uniform float time;
                uniform sampler1D paleta;
                uniform float blb;
                uniform int iter;

                out vec4 color;

                void main() {
                    vec4 z, c;

                    /*c.x = (1.3333 * (position.x) * scale);
                    c.y = ((position.y) * scale);*/

                    vec3 dir = normalize(direction);

                    int i;
                    c = vec4(0.0);
                    //c.xyw = dir;
                    //c.xzw = dir;
                    c = dir.xyzx;
                    //c.wyxz = dir.xyzy;

                    c *= scale;

                    z = c;
                    for(i=0; i<iter; i++) {
                        float nx = z.x * z.x - z.y * z.y - z.z * z.z - z.w * z.w + c.x;
                        float ny = z.x * z.y + z.y * z.w + c.y;
                        float nz = z.x * z.z - z.y * z.w + c.z;
                        float nw = z.x * z.w + z.y * z.z + c.w;

                        if((nx * nx + ny * ny + nz * nz + nw * nw) > 16.0) break;
                        z = vec4(nx, ny, nz, nw);
                    }
                    float cislo = sqrt((i == iter ? 0.0 : float(i)) / float(iter));

                    color = texture(paleta, time / 3.0 + cislo * 3.0);
                }
            )====");

            shader.compile(&camera);
        }

        void input(float dt){
            camera.createPerspective(glm::radians(FOV), (float)ulm::Window::width / (float)ulm::Window::height, 0.1f, 100000.f);

            time += dt;

            
            if(ulm::Window::keysPressed[ulm::SCANCODE_LEFT])   camera.yaw( dt * speed * scale);            
            if(ulm::Window::keysPressed[ulm::SCANCODE_RIGHT])  camera.yaw(-dt * speed * scale);                
            if(ulm::Window::keysPressed[ulm::SCANCODE_UP])     camera.pitch( dt * speed * scale);            
            if(ulm::Window::keysPressed[ulm::SCANCODE_DOWN])   camera.pitch(-dt * speed * scale);

            if(ulm::Window::wheel.dy > 0)
                scale -= scaleSpeed * dt;
            if(ulm::Window::wheel.dy < 0)
                scale += scaleSpeed * dt;
        }

        void update(int paleta){

            shader.ST_bind();

            shader.setInt("paleta", ulm::TextureManager::use(paleta));    
            shader.setInt("iter", maxIter);
            shader.setFloat("scale", scale);
            shader.setFloat("time", time);

            shader.ST_draw();
        }

        void gui(){
            if (nk_begin(ulm::NK::ctx, "SkyBrot", nk_rect(20, 400, 400, 300),
                NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
                NK_WINDOW_CLOSABLE|NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
            {
                char txt[64];

                nk_layout_row_dynamic(ulm::NK::ctx, 20, 1);                 
                sprintf(txt, "DirX: %f", camera.front.x); nk_label_wrap(ulm::NK::ctx, txt);
                nk_layout_row_dynamic(ulm::NK::ctx, 20, 1);   
                sprintf(txt, "DirY: %f", camera.front.y); nk_label_wrap(ulm::NK::ctx, txt);
                nk_layout_row_dynamic(ulm::NK::ctx, 25, 1);   
                sprintf(txt, "DirZ: %f", camera.front.z); nk_label_wrap(ulm::NK::ctx, txt);

                nk_layout_row_dynamic(ulm::NK::ctx, 30, 1); 
                sprintf(txt, "Scale: %f", scale);
                nk_label_wrap(ulm::NK::ctx, txt);

                nk_layout_row_dynamic(ulm::NK::ctx, 30, 2); 
                sprintf(txt, "FOV %.2f : ", FOV);
                nk_label_wrap(ulm::NK::ctx, txt); nk_slider_float(ulm::NK::ctx, 10.0f, &FOV, 140.f, 0.01f);

                
                nk_layout_row_dynamic(ulm::NK::ctx, 30, 2); 
                sprintf(txt, "Iterations %i : ", maxIter);
                nk_label_wrap(ulm::NK::ctx, txt); nk_slider_int(ulm::NK::ctx, 1, &maxIter, 1000, 1);

                nk_layout_row_dynamic(ulm::NK::ctx, 30, 2); 
                sprintf(txt, "Movement speed %.2f : ", speed);
                nk_label_wrap(ulm::NK::ctx, txt); nk_slider_float(ulm::NK::ctx, 0.1f, &speed, 5.f, 0.01f);

                nk_layout_row_dynamic(ulm::NK::ctx, 30, 2); 
                sprintf(txt, "Scale speed %.2f : ", scaleSpeed);
                nk_label_wrap(ulm::NK::ctx, txt); nk_slider_float(ulm::NK::ctx, 0.1f, &scaleSpeed, 100.f, 0.01f);

            }
            nk_end(ulm::NK::ctx);
        }

        ulm::String getName(){
            return ulm::String("SkyBrot");
        }
};

#endif