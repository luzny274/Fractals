#ifndef BU_SKYFRACTAL
#define BU_SKYFRACTAL

#include "header.hpp"

namespace ulm{

class SkyShaderToy : public Shader{
    private:
        static const char * ST_vertex;
        static ByteArray vertices;
        VertexArray ST_va;

        unsigned int ST_VP;

        Camera * camera;

    public:
        String code;
        
        void compile(Camera * argCamera){

            Array<unsigned int> indices(36);
            for(unsigned int i = 0; i < 36; i++)
                indices.add(i);
            Map<int, Type> attr = Map<int, Type>({3}, FLOAT);
            ST_va.setBuffers(indices, vertices, attr);

            code = String(R"====(#version 330)====") + code;
                            
            initialize(ST_vertex, code.getPtr());
            ST_VP = glGetUniformLocation(programID, "VP");

            camera = argCamera;
        }

        void ST_setVP(glm::mat4 mat){
            glUniformMatrix4fv(ST_VP, 1, GL_FALSE, glm::value_ptr(mat));
        }

        void ST_draw(){
            ST_setVP(camera->getVPWithoutTrans());
            ST_va.draw();
        }

        void ST_bind(){
            glDisable(GL_STENCIL_TEST);
            glStencilMask(0x00);

            glDisable(GL_CULL_FACE);  
            glDisable(GL_DEPTH_TEST);
            glDepthMask(GL_FALSE);

            glDisable(GL_BLEND);
            //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            bind();
        }
};

const char * SkyShaderToy::ST_vertex = R"====(
                                    #version 330

                                    layout(location = 0) in vec3 vertex;

                                    out vec3 direction;
                                    
                                    uniform mat4 VP;

                                    void main()
                                    {
                                        direction = vertex;                                        
                                        gl_Position = VP * vec4(vertex, 1.0);
                                    }
                                    )====";

ByteArray SkyShaderToy::vertices = ByteArray::parse({       -1.0f,  1.0f, -1.0f,
                                                            -1.0f, -1.0f, -1.0f,
                                                             1.0f, -1.0f, -1.0f,
                                                             1.0f, -1.0f, -1.0f,
                                                             1.0f,  1.0f, -1.0f,
                                                            -1.0f,  1.0f, -1.0f,

                                                            -1.0f, -1.0f,  1.0f,
                                                            -1.0f, -1.0f, -1.0f,
                                                            -1.0f,  1.0f, -1.0f,
                                                            -1.0f,  1.0f, -1.0f,
                                                            -1.0f,  1.0f,  1.0f,
                                                            -1.0f, -1.0f,  1.0f,

                                                             1.0f, -1.0f, -1.0f,
                                                             1.0f, -1.0f,  1.0f,
                                                             1.0f,  1.0f,  1.0f,
                                                             1.0f,  1.0f,  1.0f,
                                                             1.0f,  1.0f, -1.0f,
                                                             1.0f, -1.0f, -1.0f,

                                                            -1.0f, -1.0f,  1.0f,
                                                            -1.0f,  1.0f,  1.0f,
                                                             1.0f,  1.0f,  1.0f,
                                                             1.0f,  1.0f,  1.0f,
                                                             1.0f, -1.0f,  1.0f,
                                                            -1.0f, -1.0f,  1.0f,

                                                            -1.0f,  1.0f, -1.0f,
                                                             1.0f,  1.0f, -1.0f,
                                                             1.0f,  1.0f,  1.0f,
                                                             1.0f,  1.0f,  1.0f,
                                                            -1.0f,  1.0f,  1.0f,
                                                            -1.0f,  1.0f, -1.0f,

                                                            -1.0f, -1.0f, -1.0f,
                                                            -1.0f, -1.0f,  1.0f,
                                                             1.0f, -1.0f, -1.0f,
                                                             1.0f, -1.0f, -1.0f,
                                                            -1.0f, -1.0f,  1.0f,
                                                             1.0f, -1.0f,  1.0f
                                                    });

}

#endif