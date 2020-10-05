#include "header.hpp"

#include "fractal.cpp"
#include "mandelbrot.cpp"
#include "LuzneuxDizov.cpp"
#include "skyBrot.cpp"

ulm::Array<buf::Fractal*> fractals;

char fractalNames[1024];

void initFractals(){
    fractals.add((buf::Fractal*)new Mandelbrot());
    fractals.add((buf::Fractal*)new SkyBrot());
    fractals.add((buf::Fractal*)new LuzneuxDizovSQ());

    int ii = 0;
    for(buf::Fractal* f : fractals){
        ulm::String name = f->getName();
        for(int i = 0; i < (name.array.size - 1); i++){
            fractalNames[ii] = name[i]; ii++;
        }
        fractalNames[ii] = '\0'; ii++;
    }

}

class Main : public ulm::Program{
    public:
        float time = 0.0f;
        float dt = 0.0f;
        long int numberOfFrames = 0;

        unsigned int paleta = 0;

        int index = 0;

        bool okna = true;

        Main(){
            /* Initialization */

            ulm::Window::setVSync(false);

            int width, height, nrChannels;
            unsigned char * data = stbi_load("paleta.png", &width, &height, &nrChannels, 0);

            printf("w: %d; h: %d; %d", width, height, nrChannels);

            glGenTextures(1, &paleta);
            ulm::TextureManager::use(paleta);
            glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, width * height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_R, GL_REPEAT);

            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            free(data);

            initFractals();
        }

        void update(float deltaTime){
            /* Run each frame */

            if(time > 3.f){
                time = 0.f;
                numberOfFrames = 0;
            }

            numberOfFrames++;
            time += deltaTime;
            dt = deltaTime;
            
            if(ulm::Window::keysDown[ulm::SCANCODE_ESCAPE]) okna = !okna;

            fractals[index]->input(deltaTime);
        }
        void render(ulm::Eye eye){
            /* Render frame */

            /* 2D */
            
            fractals[index]->update(paleta);

            /* GUI */

            ulm::NK::processInput(ulm::Window::mouse, ulm::Window::wheel,
                                  ulm::Window::keysPressed, ulm::Window::keysDown, ulm::Window::keysUp, 
                                  ulm::Window::textInput);

            if(okna){
                if (nk_begin(ulm::NK::ctx, "Fractals", nk_rect(20, 20, 300, 200),
                    NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
                    NK_WINDOW_CLOSABLE|NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
                {
                    nk_layout_row_static(ulm::NK::ctx, 30, 80, 1);
                    if (nk_button_label(ulm::NK::ctx, "Exit")){
                        ulm::Window::exit();
                    }
                    nk_layout_row_dynamic(ulm::NK::ctx, 20, 1); 
                    nk_label_wrap(ulm::NK::ctx, (ulm::String("current dt: ") + dt + " ms").getPtr());

                    nk_layout_row_dynamic(ulm::NK::ctx, 20, 1); 
                    nk_label_wrap(ulm::NK::ctx, (ulm::String("average dt: ") + (time / (float)numberOfFrames) + " ms").getPtr());
                    nk_layout_row_dynamic(ulm::NK::ctx, 20, 1); 
                    nk_label_wrap(ulm::NK::ctx, (ulm::String("average framerate: ") + ((float)numberOfFrames / time) + " fps").getPtr());
                    
                    nk_layout_row_dynamic(ulm::NK::ctx, 20, 1); 
                    nk_combobox_string(ulm::NK::ctx, fractalNames, &index, fractals.size, 20, nk_vec2(110, 60));
               

                    /*nk_layout_row_dynamic(ulm::NK::ctx, 30, 2); 
                    char blbtxt[64];
                    sprintf(blbtxt, "blb %f : ", blb);
                    nk_label_wrap(ulm::NK::ctx, blbtxt); nk_slider_float(ulm::NK::ctx, 0.0f, &blb, 1.f, 0.01f);//*/

                }

                nk_end(ulm::NK::ctx);

                fractals[index]->gui();
            }

            ulm::NK::draw();
        }

        void resizeCallback(int width, int height){
            /* When resize */
            ulm::NK::setSize(width, height);
        }

        ~Main(){
            /* Destruction */
        }
};

ulm::Program * ulm::Properties::onStart(){
    ulm::Window::initialize(BU_APP_NAME, 1280, 1024);
    ulm::Window::maximize(true);
    //ulm::Window::setWindowMode(ulm::FULLSCREEN);
    return new Main();
}

void ulm::Properties::handleError(ulm::String error){
    printf("\n%s\n", error.getPtr());
    fflush(stdout);
}