#ifndef BU_LUZNEUXDIZOV
#define BU_LUZNEUXDIZOV

#include "fractal.cpp"

class LuzneuxDizovSQ : public buf::Fractal{
    private:
        ulm::ShaderToy shader;

        float scale = 1.f;
        glm::vec2 center = glm::vec2(0.5f, 0.f);
        float time = 0.f;
        int maxIter = 150;
        float a = 1.f;
        float b = 1.f;
        float c = 1.f;

        float speed = 2.f;
        float scaleSpeed = 10.f;

    public:

        LuzneuxDizovSQ(){            
            shader.code = ulm::String(R"====(
                in vec2 position;

                uniform float aspectRatio;

                uniform vec2 center;
                uniform float scale;
                uniform float time;
                uniform sampler1D paleta;
                uniform float blb;
                uniform int iter;
                uniform float a;
                uniform float b;
                uniform float c;

                out vec4 color;

                void main() {
                    vec2 posn;
                    posn.x = position.x;
                    posn.y = position.y;

                    int i = 0;
                    while(i < iter){
                        posn.x = (-a*posn.x*posn.x + a*posn.y*posn.y - c) / b;
                        posn.y = (-2.0*a*posn.x*posn.y) / b;
                        i++;
                    }

                    color = vec4(   a * posn.x * posn.x - a * posn.y * posn.y + b * posn.x + c,
                                    2.0 * a * posn.x * posn.y + b * posn.y,
                                    2.0 * a * posn.x * posn.y + b * posn.y,
                                    1.0);

                    color = abs(color);
                    color.xyz -= floor(color.xyz);
                }
            )====");

            shader.compile();
        }

        void input(float dt){
            time += dt;

            
            if(ulm::Window::keysPressed[ulm::SCANCODE_LEFT])   center.x += dt * speed * scale;            
            if(ulm::Window::keysPressed[ulm::SCANCODE_RIGHT])  center.x -= dt * speed * scale;                
            if(ulm::Window::keysPressed[ulm::SCANCODE_UP])     center.y -= dt * speed * scale;            
            if(ulm::Window::keysPressed[ulm::SCANCODE_DOWN])   center.y += dt * speed * scale;          
            if(ulm::Window::keysPressed[ulm::SCANCODE_A])      scale -= scaleSpeed * dt * scale;       
            if(ulm::Window::keysPressed[ulm::SCANCODE_S])      scale += scaleSpeed * dt * scale;

            if(ulm::Window::wheel.dy > 0)
                scale -= scaleSpeed * dt * scale;
            if(ulm::Window::wheel.dy < 0)
                scale += scaleSpeed * dt * scale;
        }

        void update(int paleta){

            ulm::Renderer2D::beginST(shader);

            shader.setInt("paleta", ulm::TextureManager::use(paleta));    
            shader.setInt("iter", maxIter);            
            shader.setFloat("scale", scale);
            shader.setFloat("time", time);
            shader.setFloat("a", a);
            shader.setFloat("b", b);
            shader.setFloat("c", c);
            shader.setVec2("center", center);

            ulm::Renderer2D::drawST(-1.f, -1.f, 2.f, 2.f);
            ulm::Renderer2D::endST();
        }

        void gui(){
            if (nk_begin(ulm::NK::ctx, "Luzneux-Dizov Square", nk_rect(20, 400, 400, 300),
                NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
                NK_WINDOW_CLOSABLE|NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
            {
                char txt[64];

                nk_layout_row_dynamic(ulm::NK::ctx, 20, 1); 
                sprintf(txt, "X: %f", center.x);
                nk_label_wrap(ulm::NK::ctx, txt);
                nk_layout_row_dynamic(ulm::NK::ctx, 20, 1); 
                sprintf(txt, "Y: %f", center.y);
                nk_label_wrap(ulm::NK::ctx, txt);
                nk_layout_row_dynamic(ulm::NK::ctx, 20, 1); 
                sprintf(txt, "Scale: %f", scale);
                nk_label_wrap(ulm::NK::ctx, txt);
                
                nk_layout_row_dynamic(ulm::NK::ctx, 30, 2); 
                sprintf(txt, "Iterations %i : ", maxIter);
                nk_label_wrap(ulm::NK::ctx, txt); 
                nk_slider_int(ulm::NK::ctx, 1, &maxIter, 1000, 1);

                nk_layout_row_dynamic(ulm::NK::ctx, 30, 2); 
                sprintf(txt, "Movement speed %.2f : ", speed);
                nk_label_wrap(ulm::NK::ctx, txt); 
                nk_slider_float(ulm::NK::ctx, 0.1f, &speed, 5.f, 0.01f);

                nk_layout_row_dynamic(ulm::NK::ctx, 30, 2); 
                sprintf(txt, "Scale speed %.2f : ", scaleSpeed);
                nk_label_wrap(ulm::NK::ctx, txt);
                nk_slider_float(ulm::NK::ctx, 0.1f, &scaleSpeed, 100.f, 0.01f);

                /* Coeficients */

                float range = 10.f;

                nk_layout_row_dynamic(ulm::NK::ctx, 30, 2); 
                sprintf(txt, "SQ A %f : ", a);
                nk_label_wrap(ulm::NK::ctx, txt); 
                nk_slider_float(ulm::NK::ctx, -range, &a, range, 0.001f);

                nk_layout_row_dynamic(ulm::NK::ctx, 30, 2); 
                sprintf(txt, "SQ B %f : ", b);
                nk_label_wrap(ulm::NK::ctx, txt); 
                nk_slider_float(ulm::NK::ctx, -range, &b, range, 0.001f);

                nk_layout_row_dynamic(ulm::NK::ctx, 30, 2); 
                sprintf(txt, "SQ C %f : ", c);
                nk_label_wrap(ulm::NK::ctx, txt); 
                nk_slider_float(ulm::NK::ctx, -range, &c, range, 0.001f);
            }
            nk_end(ulm::NK::ctx);
        }

        ulm::String getName(){
            return ulm::String("LDSq");
        }
};

#endif