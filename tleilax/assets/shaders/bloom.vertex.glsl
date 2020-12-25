// THREEjs build-in uniforms and attributes (Vertex)

// uniform mat4 modelMatrix - object.matrixWorld
// uniform mat4 modelViewMatrix - camera.matrixWorldInverse * object.matrixWorld
// uniform mat4 projectionMatrix - camera.projectionMatrix
// uniform mat4 viewMatrix - camera.matrixWorldInverse
// uniform mat3 normalMatrix - inverse transpose of modelViewMatrix
// uniform vec3 cameraPosition - camera position in world space
// attribute vec3 position - vertex position
// attribute vec3 normal - vertex normal
// attribute vec2 uv - uv
// attribute vec2 uv2 - uv2

varying vec2 vUv;

void main()
{
	vUv = uv;

	gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
}