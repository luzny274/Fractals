#ifndef BU_LUZNEUXDIZOV_2
#define BU_LUZNEUXDIZOV_2

#include "fractal.cpp"

class LuzneuxDizovSQ2 : public buf::Fractal{
    private:
        ulm::ShaderToy shader;

        float scale = 1.f;
        glm::vec2 center = glm::vec2(0.f, 0.f);
        float time = 0.f;
        int maxIter = 1;
        float a = 1.f;
        float b = 1.f;
        float c = 1.f;

        float speed = 0.5f;
        float scaleSpeed = 0.5f;

    public:

        LuzneuxDizovSQ2(){            
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
                    posn.x = scale*position.x - center.x;
                    posn.y = scale*position.y - center.y;

                    int i = 0;
                    while(i < iter){
                        posn.x = (-a*posn.x*posn.x + a*posn.y*posn.y - c) / b;
                        posn.y = (-2.0*a*posn.x*posn.y) / b;
                        i++;
                    }

                    /*color = vec4(   posn.x,
                                    posn.y,
                                    1.0,
                                    1.0);*/
                    color = vec4(   abs(atan(a * posn.x * posn.x - a * posn.y * posn.y + b * posn.x + c)),
                                    abs(atan(2.0 * a * posn.x * posn.y + b * posn.y)),
                                    abs(atan(2.0 * a * posn.x * posn.y + b * posn.y)),
                                    1.0);

                    /*color = mod(color, vec4(1.0));*/
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
            if(ulm::Window::keysDown[ulm::SCANCODE_Q])      maxIter += 1;       
            if(ulm::Window::keysDown[ulm::SCANCODE_W])      maxIter -= 1;       
            if(ulm::Window::keysDown[ulm::SCANCODE_T])      a += 1.0f;       
            if(ulm::Window::keysDown[ulm::SCANCODE_Y])      a -= 1.0f;       
            if(ulm::Window::keysDown[ulm::SCANCODE_G])      b += 1.0f;       
            if(ulm::Window::keysDown[ulm::SCANCODE_H])      b -= 1.0f;       
            if(ulm::Window::keysDown[ulm::SCANCODE_B])      c += 1.0f;       
            if(ulm::Window::keysDown[ulm::SCANCODE_N])      c -= 1.0f;

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
                nk_slider_int(ulm::NK::ctx, 1, &maxIter, 1000000, 1);

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
            return ulm::String("LDSq2");
        }
};

class LuzneuxDizovPoly : public buf::Fractal{
    private:
        ulm::ShaderToy shader;

        float scale = 1.f;
        glm::vec2 center = glm::vec2(0.f,0.f);
        int iter = 1;
        float d[100];
        int power = 1;
        int method = 1;

        float Speed = 0.5f;
        float ScaleSpeed = 0.5f;
    public:
        LuzneuxDizovPoly(){
            shader.code = ulm::String(R"====(
                in vec2 position;

                vec2 posn1;
                uniform float d[100];
                uniform int iter;
                uniform int power;
                uniform float ScaleSpeed;
                uniform float Speed;
                uniform vec2 center;
                float r;
                float f;
                uniform int method;
                 vec2 result;
                uniform float scale;
                float BIG;

                out vec4 color;

                void main() {
                    BIG = 0.0;
                    posn1.x = scale*position.x - center.x;
                    posn1.y = scale*position.y - center.y;
                    
                    r = pow((posn1.x*posn1.x+posn1.y*posn1.y),(1.0/2.0));
                    f = atan(posn1.y/posn1.x)-3.141592*(sign(posn1.x)-1.0)/2.0;
                    
                    int i = 0;
                    while(i < iter){
                        int ii = 0;
                        posn1.x = 0.0;
                        posn1.y = 0.0;
                        while(ii < power){
                            posn1.x += -d[ii]*pow(r,float(ii))*cos(float(ii)*f);
                            posn1.y += -d[ii]*pow(r,float(ii))*sin(float(ii)*f);
                            
                            ii-=-1;
                        }
                        posn1.x += d[method]*pow(r,float(method))*cos(float(method)*f);
                        posn1.y += d[method]*pow(r,float(method))*sin(float(method)*f);

                        posn1.x = posn1.x/d[method];
                        posn1.y = posn1.y/d[method];

                        r = pow((posn1.x*posn1.x + posn1.y*posn1.y),(1.0/2.0));
                        f = atan(posn1.y/posn1.x) - 3.141592*(sign(posn1.x)-1.0)/2.0;

                        r = pow(r,(1.0 / float(method)));
                        f = f / float(method);

                        if(r > 1000000000000000.0){
                            BIG = 1.0;
                            break;
                        }

                        i-=-1;
                    }
                                        
                    result.x = 0.0;
                    result.y = 0.0;
                    
                    i = 0;
                    while(i < power){
                        result.x += d[i]*pow(r,(float(i)))*cos(float(i)*f);
                        result.y += d[i]*pow(r,(float(i)))*sin(float(i)*f);
                        
                        i-=-1;
                    }
                    
                    
                    result.x = abs(atan(result.x));
                    result.y = abs(atan(result.y));
                    
                    color = vec4(
                                    result.x,
                                    result.y,
                                    BIG,
                                    1.0
                                );
                }
            )====");
            shader.compile();
        }
        void input(float dt){
            if(ulm::Window::keysPressed[ulm::SCANCODE_LEFT])   center.x += dt * Speed * scale;            
            if(ulm::Window::keysPressed[ulm::SCANCODE_RIGHT])  center.x -= dt * Speed * scale;                
            if(ulm::Window::keysPressed[ulm::SCANCODE_UP])     center.y -= dt * Speed * scale;            
            if(ulm::Window::keysPressed[ulm::SCANCODE_DOWN])   center.y += dt * Speed * scale;          
            if(ulm::Window::keysPressed[ulm::SCANCODE_A])      scale -= ScaleSpeed * dt * scale;       
            if(ulm::Window::keysPressed[ulm::SCANCODE_S])      scale += ScaleSpeed * dt * scale;          
            if(ulm::Window::keysDown[ulm::SCANCODE_Q])      iter += 1;       
            if(ulm::Window::keysDown[ulm::SCANCODE_W])      iter -= 1;

            if(ulm::Window::wheel.dy > 0)
                scale -= ScaleSpeed * dt * scale;
            if(ulm::Window::wheel.dy < 0)
                scale += ScaleSpeed * dt * scale;
        }
        void update(int paleta){
            ulm::Renderer2D::beginST(shader);

            shader.setInt("iter", iter);
            shader.setInt("power", power);
            shader.setInt("method", method);
            shader.setVec2("center", center);
            shader.setFloat("scale", scale);
            int i = 0;
            while(i < power){
                shader.setFloat(ulm::String("d[")+i+']', d[i]);
                i-=-1;
            }

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
                sprintf(txt, "Iterations %i : ", iter);
                nk_label_wrap(ulm::NK::ctx, txt); 
                nk_slider_int(ulm::NK::ctx, 1, &iter, 100, 1);

                nk_layout_row_dynamic(ulm::NK::ctx, 30, 2); 
                sprintf(txt, "Polynom maximmum power %i : ", power-1);
                nk_label_wrap(ulm::NK::ctx, txt); 
                nk_slider_int(ulm::NK::ctx, 1, &power, 100, 1);

                nk_layout_row_dynamic(ulm::NK::ctx, 30, 2); 
                sprintf(txt, "Iteration method %i : ", method);
                nk_label_wrap(ulm::NK::ctx, txt); 
                nk_slider_int(ulm::NK::ctx, 1, &method, power, 1);

                nk_layout_row_dynamic(ulm::NK::ctx, 30, 2); 
                sprintf(txt, "Movement speed %.2f : ", Speed);
                nk_label_wrap(ulm::NK::ctx, txt); 
                nk_slider_float(ulm::NK::ctx, 0.1f, &Speed, 5.f, 0.01f);

                nk_layout_row_dynamic(ulm::NK::ctx, 30, 2); 
                sprintf(txt, "Scale speed %.2f : ", ScaleSpeed);
                nk_label_wrap(ulm::NK::ctx, txt);
                nk_slider_float(ulm::NK::ctx, 0.1f, &ScaleSpeed, 10.f, 0.01f);

                /* Coeficients */

                float range = 10.f;
                int i = 0;
                while(i < power){

                    nk_layout_row_dynamic(ulm::NK::ctx, 30, 2); 
                    sprintf(txt, "Polynome d_%d %f : ", i, d[i]);
                    nk_label_wrap(ulm::NK::ctx, txt); 
                    nk_slider_float(ulm::NK::ctx, -range, &d[i], range, 0.001f);

                    i-=-1;
                }
            }
            nk_end(ulm::NK::ctx);
        }
        ulm::String getName(){
            return ulm::String("Luznuex-Dizov Polynom");
        }
};
#endif