#version 450
#
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColour; 
layout(location = 0) out vec3 fragColour;

layout( push_constant ) uniform constants{
    mat4 wvp; 
} PushConstants;

void main() {
    gl_Position = PushConstants.wvp * vec4(inPosition, 1.0);
    fragColour = vec3(inColour.x, inColour.y, inColour.z);

}
