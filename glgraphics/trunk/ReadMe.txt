TODO: Update everything below, with descriptions for each class
GLGraphics provides some miscellaneous general-purpose OpenGL utility code. Included modules:

	* GLGraphics defines GLGraphics_init, which is to be called at startup (by the shell, if applicable) to initialize glew (if applicable) and define the OpenGL API version, which can later be retrieved with GLGraphics_getOpenGLAPIVersion.

	* GLIncludes.h is a convenience header that #includes the appropriate OpenGL headers for the target platform.

	* GLSLShader provides simplified vertex and fragment shader setup.

	* GLUtilities provides OpenGL utility functions for translating between string representations and GLenum representations of OpenGL constants.




SCRATCHPAD

Mesh \
	Vertices \
		Position
		Normal
		Tangent
		Color
		Texcoords
		Bone IDs (4)
		Bone weights (4)
Armature \
	Bone hierarchy \
		Bone ID
		Transform relative to parent
Morph \
	Vertices \
		Position offset
		Normal
		Tangent
Animation \
	Armature reference
	Morph reference
	Frame rate
	Frames \
		Bone transforms \
			Bone ID
			Transformation matrix
		Morph weights \
			Morph mesh ID (?)
			Weight
AnimationState \
	Armature reference
	Bone transform list
	Morph reference list
	Morph weight list
MISSING: Animation collection, AnimationStateController or equivalent, animation transitions

Shape keys/morph targets are pretty tricky. Ideas:
- Organize vertices into groups which own morph sets, up to some limit in the vertex shader. Different glDrawElements calls for each group.
* Calculate all morphs in software, and resubmit vertex data every frame for animations that have morphs (only if dirty?).
- Limit entire mesh to some number of morphs, and encode all morph offsets into static vertex data with uniforms for weights.
- Encode vertex data as normal, but calculate offsets for everything at once in software and pass those in some other way. Sampler?
 - This is essentially no better than resubmitting vertex data every frame, since it contains a full list of positions/offsets anyway

Constraints and control objects are Blender-only. Exported animations can be flattened.

MATERIAL STUFF
- Color map (two layers?)
- Specular map
- Normal map
- Displacement map
- Occlusion map
- Subsurface scattering (research needed)
- PBR
 - Metalness
 - Roughness
 - AO


TODO:
- Animation controller object with graph logic? Also event dispatching
- IK (https://www.webcitation.org/60uCFHqC2?url=http://freespace.virgin.net/hugo.elias/models/m_ik2.htm)
- Advanced lighting (research needed)
 - Idea: Keep multiple fragment shaders compiled and ready; on CPU, compute the number of influencing lights for each object, and select shader with appropriate number of lights in it to draw that object, up to a reasonable maximum
- Depth sorting/translucency
- Fluid simulation
- VR

LOWER PRIORITY:
- Camera work:
 - Camera interpolation (controller object interpolates between different camera objects)
 - Other cameras: First person, follow behind/ahead
- Morphing? (maybe better to do without)
- Possible normal map optimization: Do all lighting calculations in tangent space, so fragment shader doesn't have to do a matrix multiplication per fragment
- Calculate normal matrix in renderer instead of vertex shader
- Callback renderable type
- Culling
- Fog
- Shadows
- SSAO
- Cloth simulation
