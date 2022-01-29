R"(
precision mediump float;
uniform sampler2D uTexture0;
varying vec2 vTexCoord;
uniform vec2 uSrcTexSize;
uniform vec2 uDstTexSize;
uniform int uScale;
        
vec4 do_resize(vec2 uv, vec2 startWeight, vec2 endWeight) {       
    vec4 fragColor = vec4(0.0);                                
    float totalWeight = 0.0;                                   
    for (int i = 0; i < 4; i++) {                        
        float x_weight = 1.0;                                  
        if (i == 0) {                                          
            x_weight = startWeight.x;                          
        } else if (i == uScale - 1) {                         
            x_weight = endWeight.x;                            
        } else if (i == uScale) {
            break;
        }                                                     
        for (int j = 0; j < 4; j++) {                    
            float y_weight = 1.0;                              
            if (j == 0) {                                      
                y_weight = startWeight.y;                      
            } else if (j == uScale - 1) {                     
                y_weight = endWeight.y;                        
            } else if (j == uScale) {
                break;
            }                                                
            vec2 srcUV = (uv + vec2(i, j)) / uSrcTexSize;      
            float weight = x_weight * y_weight;                
                                                               
            fragColor += weight * texture2D(uTexture0, srcUV); 
            totalWeight += weight;                             
        }                                                      
    }                                                          
    return fragColor / totalWeight;                            
}

void main() {
    vec2 scale = uSrcTexSize / uDstTexSize;
    vec2 srcFragCoordStart = floor(vTexCoord * uDstTexSize) * scale;
    vec2 srcFragCoordEnd = srcFragCoordStart + scale;
    vec2 startWeight = floor(srcFragCoordStart + 1.0) - srcFragCoordStart;
    vec2 endWeight = (srcFragCoordEnd)-ceil(srcFragCoordEnd - 1.1);
    gl_FragColor = do_resize(srcFragCoordStart, startWeight, endWeight);
}
)"
