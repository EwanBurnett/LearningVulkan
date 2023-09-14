echo "Compiling Shaders for Hello-Triangle"
glslc Shaders/Triangle.vert -o build/Examples/Hello-Triangle/vert.spv
glslc Shaders/Triangle.frag -o build/Examples/Hello-Triangle/frag.spv

echo "Compiling Shaders for VertexBuffer"
glslc Shaders/VertexBuffers.vert -o build/Examples/Vertex-Buffers/vert.spv
glslc Shaders/VertexBuffers.frag -o build/Examples/Vertex-Buffers/frag.spv


